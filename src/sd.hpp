/**
 * @file sd.hpp
 * @author anb78
 *
 * @brief SD card-related definitions for air sampler
 */
#ifndef SD_HPP
#define SD_HPP

#include "f_util.h"
#include "ff.h"
#include "hw_config.h"
#include <string>

/**
 * Container for SD card-related functionality.
 */
class SD {
public:
    /**
     * Begins the SD card interface by mounting the SD card.
     */
    bool begin();
    
    /**
     * Logs a mode transition event to the SD card.
     */
    bool log_transition(uint32_t time_ms, const std::string& from_mode, const std::string& to_mode 
                        );
    
    /**
     * Closes any open files and unmounts the SD card.
     */
    void end();
    
private:
    /**
     * Creates a new log file with headers if it doesn't exist
     */
    bool ensure_log_file();
    
    /**
     * File system representation
     */
    FATFS fs;
    
    /**
     * Return value for SD operations
     */
    FRESULT fr;
    
    /**
     * Log file object representation
     */
    FIL log_file;
    
    /**
     * Whether the SD card is initialized
     */
    bool initialized = false;
    
    /**
     * The current log filename
     */
    char log_filename[32] = "air_sampler_log.csv";
};

// Global SD card object
extern SD sd_card;

#endif // SD_HPP