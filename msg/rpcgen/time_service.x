/*
 * time_service.x - RPC definition for a simple time service.
 */

enum ProcID {
    GET_TIME = 1,
    SET_TIME = 2
};

program TIMESERVICE_PROG {
    version TIMESERVICE_VERS {
        string get_time(void) = GET_TIME;
        void set_time(string) = SET_TIME;
    } = 1;
} = 0x20000001;
