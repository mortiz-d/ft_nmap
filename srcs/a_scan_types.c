#include "../lib/nmap.h"

void scan_syn(){
//tcp syn
//send a tcp SYN packet, wait for a response
//      SYN/ACK or just SYN -> port open
//      RST (reset) -> closed
//      no response -> filtered
//      ICMP unreach (type 3, code 0, 1, 2, 3, 9, 10, or 13) -> filtered
}

void scan_udp(){
    //udp
}

void scan_null(){
//tcp
}

void scan_fin(){
//tcp
}

void scan_xmas(){
//tcp
}

void scan_ack(){
    //tcp
}
