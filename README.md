# NCUC2.0_MASTER
类EtherCAT协议——NCUC2.0主站协议栈开发

gcc test2.c datagram.c format.c frame.c -o dd  
./dd  
使用寄存器读写函数请参考socket_test  
gcc socket_test.c datagram.c format.c frame.c bufferSendrecv.c register.c -o socket_test
  
只要使用了寄存器读写函数，或者根本上讲是使用了报文收发函数，都需要按照socket_test.c示例的样子创建套接字，最终关闭套接字等

register_operation_test.c中是关于寄存器读写的测试

过程数据通道访问：
gcc bufferSendrecv.c channel_operation_test.c datagram.c format.c frame.c process_data_channel.c register.c reverse_array.c  -o ch

服务请求应答通道访问：
gcc bufferSendrecv.c  channel_view.c datagram.c format.c frame.c register.c reverse_array.c service_req_resp_channel.c test_req_resp_ch.c  -o ch

通道映射：
gcc bufferSendrecv.c datagram.c format.c frame.c mapping_config_test.c mapping.c register.c -o mapping_config_test