#include <stdio.h>
#include <assert.h>
#include <sync.h>
#include <configuration.h>
#include <file-properties.h>
#include <processes.h>
#include <unistd.h>

/*!
 * @brief main function, calling all the mechanics of the program
 * @param argc its number of arguments, including its own name
 * @param argv the array of arguments
 * @return 0 in case of success, -1 else
 * Function is already provided with full implementation, you **shall not** modify it.
 */
int main(int argc, char *argv[]) {
    // Check parameters:
    // - source and destination are provided
    // - source exists and can be read
    // - destination exists and can be written OR doesn't exist but can be created
    // - other options with getopt (see instructions)
    configuration_t my_config;
    init_configuration(&my_config);
    if (set_configuration(&my_config, argc, argv) == -1) {
        return -1;
    }

    // Check directories
    if (!directory_exists(my_config.source) || !directory_exists(my_config.destination)) {
        printf("Either source or destination directory do not exist\nAborting\n");
        return -1;
    }
    // Is destination writable?
    if (!is_directory_writable(my_config.destination)) {
        printf("Destination directory %s is not writable\n", my_config.destination);
        return -1;
    }

    // Prepare (fork, MQ) if parallel
    process_context_t processes_context;
    prepare(&my_config, &processes_context);

    // Run synchronize:
    synchronize(&my_config, &processes_context);
    
    // Clean resources
    clean_processes(&my_config, &processes_context);

    return 0;
}
