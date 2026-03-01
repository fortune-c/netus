//
// Created by fortune on 3/1/26.
//

#ifndef WEB_SERVER_FILES_H
#define WEB_SERVER_FILES_H

unsigned long get_file_size(char *path);
void write_file(char *path, void *data, unsigned long size);
void append_file(char *path, void *data, unsigned long size);
void * read_file(char *path);

#endif //WEB_SERVER_FILES_H