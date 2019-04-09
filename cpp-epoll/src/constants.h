//
// Created by Artem on 31.12.2018.
//

#ifndef HIGHLOAD_CONSTANTS_H
#define HIGHLOAD_CONSTANTS_H

const int yes = 1;
const int buffer_size = 1024 * 512;
const int read_buffer_size = 1024 * 8;
const int max_events = 1000;
const int pages_count = 23436;
const int max_records_count = pages_count * 64;

#define PORT 3005

#endif //HIGHLOAD_CONSTANTS_H
