#ifndef page_table_h
#define page_table_h

struct page_table_entry{
	int frame_number;
};

struct page_table{
    struct page_table_entry *table;
    int size;
};

struct page_table* initialise_page_table(FILE *script, int num_lines);
int get_page_size();
int get_frame_size();

#endif