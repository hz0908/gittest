void bufferPrint(const char* buffer, size_t buffer_size);
int socketCreate(char *networkport);
int send_socketCreate(char *networkport);
int rev_socketCreate(char *networkport);
int socketClose(int sock_fd);
int send_rev_socketClose();
int send_buffer(char *transbuffer,const unsigned short buffer_length);
int rev_buffer(char *buffer, int timeout);