#include "utils.h"


void log_error(const char* message) {
#if LOG_DEBUG
    fprintf(stderr, "DEBUG: %s\n", message);
#endif
}