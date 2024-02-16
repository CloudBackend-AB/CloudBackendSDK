/*
    CloudBackend AB 2022 - 2024.
*/

#include "partition_large_csv_file.h"

#include "cbe/QueryChainSync.h"

#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>    // read & write files
#include <iomanip>    // std::setw
#include <ios>        // std::left, right
#include <iostream>
#include <string>     // std::string::substr, std::stoi,
#include <thread>

bool debug_this = false;    // if to print trace output
bool finished = false;      // if the whole program work is finished 
bool upload_done = false;   // if the file upload has been confirmed
bool upload_error = false;  // error occurred during upload
float per_upload;           // time per upload
float per_row;              // time per row uploaded in chunks
int line_header = 0;        // csv line containing the column header, first is 0
int lines_read = 0;         // number of lines processed
int data_add  = 100000000;  // addition to representing the data set
int data_mult = 1000000;    // multiplier representing the data set
int data_set  = 0;          // csv file can contain multiple data sets where the row label numbers starts over
int row_begin = 0;          // csv row number after the header to start uploading, first is 0
int row_count = 999999;     // number of rows to upload
int objects_written = 0;    // count of number of objects written
int object_number;          // calculate numeric title of object
int chunk_size = 50000;     // number of lines per object to write data
int line_number = 0;        // line number read from input file
int lines_written = 0;      // number of lines written in this chunk
int total_lines_written = 0;// total number of all lines written
std::ifstream indata_file;  // local file to read from
std::ofstream outdata_file; // local file to write to
std::time_t time_begin;     // start time
std::time_t time_begin_upl; // start time of one upload
std::time_t time_finish;    // end time
std::time_t time_period;    // duration from start to end time
std::string line;           // one line read from the csv input file
std::string column_headers; // csv column headers
std::string row_label;      // first column of line read, contains the row number in a data set
std::string object_title;   // object title name
std::string object_first;   // title name of first object
std::string object_last;    // title name of last object
std::string object_col_head = "+";   // title prefix to name of the column header
std::string cbe_container   = "";
std::string csv_file        = "./test.csv"; // input file
std::string uc_dir          = "../";        // user credentials directory
std::string uc_file         = "user_credentials.cpp"; // user credentials file
std::string local_temp_dir  = "/tmp/"; // directory for temporary files to be uploaded, must end with /
std::string filename;       // local file name with extension .csv
std::string recent_file;    // next file to delete
std::string path_filename;  // full filename with path
std::string stemp;          // string for temporary manipulation
std::string username, password, tenant, client;

void waitUntilUploaded() {
  while (!upload_done)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void waitUntilFinished() {
  while (!finished)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }
}

void programFinished() {
  std::cout << "program complete! ";
  finished = true;
}

/* Functions */
void this_login() {
  std::string line;
  std::ifstream ucfile;
  std::size_t found;
  std::size_t pos1;
  std::size_t pos2;
  std::string temp_username, temp_password, temp_tenant, temp_client;
  std::string uc_temp = uc_dir.append(uc_file);
  int wpos;
  int wlength;
  bool uc_found = false;
  if (uc_file > " ") ucfile.open(uc_temp);
  if (ucfile.is_open()) {
    while (getline(ucfile, line)) {
    //  if (debug_this) {
    //    std::cout << "line: " << line << '\n';
    //  }
      found=line.find("username");
      if (found!=std::string::npos) {
        pos1=line.find('"');
        wpos=pos1 + 1;
        pos2=line.find('"',wpos);
        wlength=pos2 - wpos;
        temp_username=line.substr(wpos,wlength);
        uc_found = true;
        if (debug_this) {
          std::cout << "username: " << temp_username << '\n';
        }
      }
      found=line.find("password");
      if (found!=std::string::npos) {
        pos1=line.find('"');
        wpos=pos1 + 1;
        pos2=line.find('"',wpos);
        wlength=pos2-wpos;
        temp_password=line.substr(wpos,wlength);
        uc_found = true;
        if (debug_this) {
          std::cout << "password: ***" << '\n';
          // std::cout << "password: " << temp_password << '\n';
        }
      }
      found=line.find("tenant");
      if (found!=std::string::npos) {
        pos1=line.find('"');
        wpos=pos1 + 1;
        pos2=line.find('"',wpos);
        wlength=pos2-wpos;
        temp_tenant=line.substr(wpos,wlength);
        uc_found = true;
        if (debug_this) {
          std::cout << "tenant  : " << temp_tenant << '\n';
        }
      }
      found=line.find("client");
      if (found!=std::string::npos) {
        pos1=line.find('"');
        wpos=pos1 + 1;
        pos2=line.find('"',wpos);
        wlength=pos2-wpos;
        temp_client=line.substr(wpos,wlength);
        uc_found = true;
        if (debug_this) {
          std::cout << "client  : " << temp_client << '\n';
        }
      }
    }
    ucfile.close();
  } else {
      if (uc_file > " ") std::cout << "can not find the uc file: " 
                                   << uc_temp << std::endl;
  }
  if (debug_this) {
    std::cout << "uc: " << uc_temp << "\t"
              << "u: "  << temp_username  << "\t"
          //  << "p: "  << temp_password  << "\t"
              << "t: "  << temp_tenant    << "\t"
              << std::endl;
  }

  if (uc_found) {
    if (temp_username.empty()) {
      std::cout << "Username: ";
      std::getline(std::cin, username);
    } else {
      username = temp_username;
    }
    if (temp_password.empty()) {
      std::cout << "Password: ";
      std::getline(std::cin, password);
    } else {
      password = temp_password;
    }
    if (temp_tenant.empty()) {
      std::cout << "Tenant  : ";
      std::getline(std::cin, tenant);
    } else {
      tenant   = temp_tenant;
    }
    if (temp_client.empty()) {
      std::cout << "Client  : ";
      std::getline(std::cin, client);
    } else {
      client   = temp_client;
    }
  } else {
    std::cout << "Login to your CloudBackend tenant user account:" << std::endl;

    std::cout << "Username: ";
    std::getline(std::cin, username);

    std::cout << "Password: ";
    std::getline(std::cin, password);

    std::cout << "Tenant  : ";
    std::getline(std::cin, tenant);

    std::cout << "Client  : ";
    std::getline(std::cin, client);
  }

}

void read_csv(cbe::Container this_container) {
  std::ifstream indata_file;
  std::size_t found;
  std::size_t pos1;
  std::size_t pos2;
  int stop_after_line_number = row_begin + row_count;
  int row_group = 0;
  int row_number;
  int wpos, wlength;
  bool file_full = false;

  if (csv_file > " ") indata_file.open(csv_file);
  if (indata_file.is_open()) {
    while (getline(indata_file, line)) {
      if (line_number == line_header) {
        column_headers = line;
        /* write column headers to file */
        path_filename = local_temp_dir;
        object_number = data_add + row_begin;
        stemp  = std::to_string(object_number);
        object_title  = stemp.substr(2);
        filename = object_col_head.substr(0,1);
        filename.append(object_title);
        filename.append(".csv");
        object_col_head = filename;
        path_filename.append(filename);
        if (debug_this) {
          std::cout << "About to write to: " << path_filename << std::endl;
        }
        if (!outdata_file.is_open()) {
          outdata_file.open(path_filename);
        } 
        if (outdata_file.is_open()) {
          outdata_file << line << "\n";
          outdata_file.close();
        } else {
          std::cout << "can not open the output file: " 
                    << filename << std::endl;
        }
        upload_file(this_container);
        objects_written = 0;
        total_lines_written = 0;
        ++data_set;
      }  // writing and uploading column header file

      found=line.find(',');
      if (found!=std::string::npos) {
        pos1=line.find(',');
        row_label=line.substr(0, pos1);
        if (row_label.empty()) {
          row_number=-1;
        } else {
          // row_number=std::stoi(row_label);
          row_number=lines_read;
        }
      }
      if (row_label == "0") {
        ++data_set;
      }
      // object_number = data_add + (data_set*data_mult) + row_number;
      object_number = data_add + (data_set*data_mult) + lines_read + 1;
      stemp  = std::to_string(object_number);

      object_title  = stemp.substr(1);
      if (debug_this && row_label.empty()) {
        std::cout << "column headers: "
                  << column_headers.substr(0,60) << std::endl;
        std::cout << "   line #" 
                  << "\tset row num" 
                  << "\tobject"   
                  << std::endl;

        std::cout << "---------" 
                  << "\t--- -------" 
                  << "\t--------" 
                  << std::endl;
      }
      if (line_number > row_begin) {
        if (object_first.empty()) {
          object_first = object_title;
        }
        /* if no output file is open start a new file */
        if (!outdata_file.is_open()) {
          path_filename = local_temp_dir;
          filename = object_title;
          filename.append(".csv");
          path_filename.append(filename);
          object_last = object_title;
          if (debug_this) {
            std::cout << "About to write to: " << path_filename << std::endl;
          }
          outdata_file.open(path_filename);
          lines_written = 0;
        } 
        /* write a row to chunk file */
        if (outdata_file.is_open()) {
          outdata_file << line << "\n";
          ++lines_written;
          if ((lines_written == chunk_size) 
                                   || (line_number == stop_after_line_number)) {
            outdata_file.close();
          }
        } else {
          std::cout << "can not open the output file: " 
                    << filename << std::endl;
        }

        /* is it time to upload the output data file ? */
        if (!outdata_file.is_open()) {
          upload_file(this_container);
          ++data_set;
          if (debug_this) std::cout << "dataset: " << data_set << std::endl;
        } // upload object
      }
      lines_read = line_number;
      ++line_number;
      if (upload_error) {
        indata_file.close();
        std::cout << "Read aborted! " << std::endl;
        return;
      }
      if (line_number > stop_after_line_number) {
        indata_file.close();
        std::cout << "Read reached -n limit. " 
                  << "Lines in last: " 
                  << lines_written << std::endl;
        return;
      }
    }
    indata_file.close();
  } else {
      if (csv_file > " ") {
        std::cout << "can not open the csv file: " 
                  << csv_file << std::endl;
      }
  }

  if (outdata_file.is_open()) {
    outdata_file.close();
    upload_file(this_container);
  } // upload object
  std::cout << "Read all of the input file. "
            << "Lines in last: " 
            << lines_written << std::endl;
}

void upload_file(cbe::Container this_container) {
  /* upload output file just written */
  if (debug_this) {
  std::cout << "About to upload: " 
            << local_temp_dir << filename << std::endl;
  }
  upload_done = false;
  time_begin_upl  = std::time(nullptr);
  uploadToContainer(local_temp_dir, filename, this_container);
  if (!recent_file.empty()) {
  stemp = "rm ";
  stemp.append(recent_file);
  if (debug_this) {
      std::cout << stemp << std::endl;
  }
  std::system(stemp.c_str());  // delete recent object file
  }
  recent_file = path_filename;
  // waitUntilUploaded();
  if (!upload_error) {
    ++objects_written;
    total_lines_written += lines_written;
    if (debug_this) {
      std::cout << std::right << std::setw(9) << line_number << ":\t" 
                << std::right << std::setw(3) << data_set 
                << std::right << std::setw(8) << row_label << "\t" 
                << object_title << std::endl;
    }
  }
}

void write_summary(cbe::Container this_container) {
  std::string path_filename, stemp;

  /* write summary to file */
  path_filename = local_temp_dir;
  filename = "z-content";
  filename.append(".txt");
  path_filename.append(filename);
  if (debug_this) {
    std::cout << "About to write to: " << path_filename << std::endl;
  }
  if (!outdata_file.is_open()) {
    outdata_file.open(path_filename);
  } 
  time_finish = std::time(nullptr);
  time_period = time_finish - time_begin;
  per_upload  = (float) time_period / objects_written;
  per_row     = (float) time_period / total_lines_written;
  if (outdata_file.is_open()) {
    outdata_file << "SUMMARY" << std::endl;
    outdata_file << "data source     : "  << csv_file               << std::endl;
    outdata_file << "lines read      : "  << lines_read             << std::endl;
    outdata_file << "column headers  : "  << object_col_head        << std::endl;
    outdata_file << "first up        : "  << object_first << ".csv" << std::endl;
    outdata_file << "last  up        : "  << object_last  << ".csv" << std::endl;
    outdata_file << "object splits   : "  << objects_written        << std::endl;
    outdata_file << "max lines/object: "  << chunk_size             << std::endl;
    outdata_file << "total uploaded  : "  << total_lines_written    << std::endl;
    outdata_file << "program start   : "
                 << std::asctime(std::localtime(&time_begin));
    outdata_file << "program end     : "
                 << std::asctime(std::localtime(&time_finish));
    outdata_file << "time duration   : "  << time_period << " s" 
                 << std::endl;
    outdata_file << "per upload      : "  << per_upload << " s" 
                 << std::endl;
    outdata_file << "per row uploaded: "  << per_row << " s" 
                 << std::endl;
    outdata_file.close();
  } else {
    std::cout << "can not open the output file: " << filename << std::endl;
  }

  /* upload object */
  if (debug_this) {
    std::cout << "About to upload: " << local_temp_dir << filename << std::endl;
  }
  upload_done = false;
  time_begin_upl  = std::time(nullptr);
  uploadToContainer(local_temp_dir, filename, this_container);
  if (!recent_file.empty()) {
    stemp = "rm ";
    stemp.append(recent_file);
    if (debug_this) {
      std::cout << stemp << std::endl;
    }
    std::system(stemp.c_str()); // delete last object file
  }
  recent_file = path_filename;

  // waitUntilUploaded();

  stemp = "cat ";
  stemp.append(path_filename);
  std::system(stemp.c_str()); // show last object file

  if (debug_this) {
    std::cout << time_begin  << std::endl;
    std::cout << time_finish << std::endl;
    std::cout << "summary file: " 
              << path_filename
              << " uploaded to container "
              << cbe_container
              << std::endl;
  }
  programFinished();
}

//////////////////////////////////////// main //////////////////////////////////
int main(int argc, char *argv[]) {
  bool next_has_value;
  int arg_check = argc - 1;
  std::string this_arg, next_arg;

  for (int i = 0; i < argc; ++i) {
    this_arg = std::string(argv[i]);
    if (i < arg_check) {
      next_arg = std::string(argv[i+1]);
      if (next_arg.at(0) == '-') {
        next_has_value = false;
      } else {
        next_has_value = true;
      }
    }
    if(this_arg == "-?" || this_arg == "--help") {
      std::cout << "\n"
                << "usage: " << argv[0] 
                << " -csv data_file"
                << " -header line"
                << " -from row"
                << " -n maximum_rows_written"
                << " -chunk size"
                << " -uc user_credentials_file"
                << " -container store"
                << "\n\n"
                << "-csv data_file \t\t.csv file name to read as indata, for current directory use prefix ./ \n"
                << "-header line_number \tline with csv column headings, default 0 \n"
                << "-from row_number \trow to begin with, skip rows until, default 0 \n"
                << "\t\t\tfirst line should contain the column headings \n"
                << "\t\t\tsecond line is row number 0 \n"
                << "-n number_of_rows\tthe number of rows to upload, default is all to the end of the csv-file \n"
                << "-chunk max chunk size \te.g. rows per object file, default 50 000 \n"
                << "-uc user_credentials \tfile in directory .cbe with login to CloudBackend\n"
                << "-container name \tthe CloudBackend full container path and name to store the data objects\n\n"
                << "Example : ./partition_large_csv_file -csv test.csv -chunk 1000 -container z-Choped"
                << std::endl;
      return 0;
    }
    if(argv[i] == std::string("-csv")) {
      if (next_has_value) {
        csv_file = next_arg;
      }
    }
    if(argv[i] == std::string("-header")) {
      if (next_has_value) {
        line_header = std::stoi(next_arg);
      }
    }
    if(argv[i] == std::string("-from")) {
      if (next_has_value) {
        row_begin = std::stoi(next_arg);
      }
    }
    if(argv[i] == std::string("-n")) {
      if (next_has_value) {
        row_count = std::stoi(next_arg);
      }
    }
    if(argv[i] == std::string("-chunk")) {
      if (next_has_value) {
        chunk_size = std::stoi(next_arg);
      }
    }
    if(argv[i] == std::string("-uc")) {
      if (next_has_value) {
        uc_file = next_arg;
      }
    }
    if(argv[i] == std::string("-container")) {
      if (next_has_value) {
        cbe_container = next_arg;
      }
    }
    if(this_arg == "-d" || this_arg == "--debug") {
      debug_this = true;
    }

  }

  if (debug_this) {
    std::cout << "user credentials: \t'" << uc_file       << "'" << std::endl;
    std::cout << "csv file: \t\t'"       << csv_file      << "'" << std::endl;
    std::cout << "container: \t\t'"      << cbe_container << "'" << std::endl;
    std::cout << "line_header: \t\t'"    << line_header   << "'" << std::endl;
    std::cout << "row_begin: \t\t'"      << row_begin     << "'" << std::endl;
    std::cout << "row_count: \t\t'"      << row_count     << "'" << std::endl;
  }
  if (cbe_container.empty()) {
    std::cout << "Error: CloudBackend container with full path has to be specified. "
    << "help: " << argv[0] << " -?"
    << std::endl;
    return 1;
  }
  if (cbe_container == "/") {
    std::cout << "Error: container for this usage may not be the root container." << std::endl;
    return 1;
  }

  time_begin = std::time(nullptr);
  this_login();

  if (debug_this) {
    std::cout << "About to login as: " << username << std::endl;
  }
  cbe::CloudBackend::LogInError logInError;
  auto cloudBackend = cbe::CloudBackend::logIn(username, password, tenant, client, logInError);
  std::cout << "login as: " << cloudBackend.account().username() << std::endl;

  if (logInError){
    std::cout << "Error, login failed! \nError info="
              << logInError << std::endl;
    return 2;
  }

  cbe::Container parentContainer = cloudBackend.account().rootContainer();
  queryContainer(parentContainer, cbe_container);

  std::cout << "Goodbye." << std::endl;
  cloudBackend.terminate();
  return 0;
}

//////////////////////////////////////// main end //////////////////////////////

//Server requests

void queryContainer(cbe::Container parentContainer, std::string name) {
  std::cout << "About to query container." << std::endl;
  _queryName = name;
  cbe::Container::QueryError queryError;
  auto queryResult = parentContainer.query(queryError);
  if (queryError) {
    std::cout << "Error! " << queryError << std::endl;
    return;
  }

  cbe::QueryResult::ItemsSnapshot resultset = queryResult.getQueryResult().getItemsSnapshot();
  // Look through the parent container to check if the name has already been 
  // used.
  cbe::Container subContainer{cbe::DefaultCtor{}};
  for (cbe::Item& item : resultset)
  {
    // std::cout << item.name() << std::endl;

    if (debug_this) {
      std::cout << "comparing " << item.name() << " " 
                << _queryName << std::endl;
    }
    if (item.name() == _queryName){
      std::cout << "Container already exist!  " << item.name() << " (" 
                << item.id() << ")" << std::endl;
      subContainer = cbe::CloudBackend::castContainer(item);
    }
  }

  if (subContainer) {
    std::cout << "Using existing container: " << subContainer.name()
              << " (" << subContainer.id() << ")" << std::endl;
  } else {
    subContainer = createContainer(parentContainer, _queryName);
  }
  read_csv(subContainer);
  write_summary(subContainer);
}

void uploadToContainer(std::string path, std::string name, cbe::Container container) {
  if (debug_this) {
    std::cout << "About to upload to container: " << container.name() << std::endl;
  }
  cbe::Container::UploadError uploadError;
  container.upload(name, path, uploadError);
  if (uploadError) {
    std::cout << "Error! " << uploadError << std::endl;
    return;
  }
}

cbe::Container createContainer(cbe::Container container, std::string name) {
  std::cout << "Create container: " << name << " located in container: " << container.name() << " (" << container.id() <<")" << std::endl;
  cbe::Container::CreateContainerError createContainerError;
  auto newContainer = container.createContainer(name, createContainerError);
  if (createContainerError) {
    std::cout << "Error! " << createContainerError << std::endl;
  }
  return *newContainer;
}


