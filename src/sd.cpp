/**
 * @file sd.cpp
 * @author anb78
 *
 * @brief SD card-related functionality for air sampler
 */

 #include "sd.hpp"
 #include <cstdio>
 #include <ctime>
 
 // Initialize the global SD card object
 SD sd_card;
 
 bool SD::begin() {
     // Mount the SD card filesystem
     fr = f_mount(&fs, "", 1);
     
     if (fr != FR_OK) {
         printf("SD Mount Error: %s (%d)\n", FRESULT_str(fr), fr);
         initialized = false;
         return false;
     }
     
     // Create the log file with headers if it doesn't exist
     if (!ensure_log_file()) {
         printf("Failed to create log file\n");
         initialized = false;
         return false;
     }
     
     printf("SD card initialized successfully\n");
     initialized = true;
     return true;
 }
 
 bool SD::ensure_log_file() {
     // Try to open the file to check if it exists
     fr = f_open(&log_file, log_filename, FA_READ);
     
     if (fr == FR_OK) {
         // File exists, close it
         f_close(&log_file);
         return true;
     }
     
     // File doesn't exist, create it with headers
     fr = f_open(&log_file, log_filename, FA_WRITE | FA_CREATE_ALWAYS);
     if (fr != FR_OK) {
         printf("SD Open Error: %s (%d)\n", FRESULT_str(fr), fr);
         return false;
     }
     
     // Write headers
     const char* headers = "system_time_ms,from_mode,to_mode,notes\n";
     if (f_printf(&log_file, headers) < 0) {
         printf("SD Write Error\n");
         f_close(&log_file);
         return false;
     }
     
     f_close(&log_file);
     return true;
 }
 
 bool SD::log_transition(uint32_t time_ms,const std::string& from_mode, const std::string& to_mode
                       ) {
     if (!initialized) {
         printf("SD card not initialized\n");
         return false;
     }
     
     // Open the log file for appending
     fr = f_open(&log_file, log_filename, FA_OPEN_APPEND | FA_WRITE);
     
     if (fr != FR_OK) {
         printf("SD Open Error: %s (%d)\n", FRESULT_str(fr), fr);
         return false;
     }
     
     // Format: timestamp,system_time,from_mode,to_mode,notes
     int result = f_printf(&log_file, "%u,%s,%s,mode_transition\n", 
                            time_ms, from_mode.c_str(), to_mode.c_str());
     
     f_close(&log_file);
     
     if (result < 0) {
         printf("SD Write Error\n");
         return false;
     }
     
     return true;
 }
 
 void SD::end() {
     if (initialized) {
         f_unmount("");
         initialized = false;
     }
 }