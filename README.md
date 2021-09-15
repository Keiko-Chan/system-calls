# system-calls
# Second course

Stat - вносит в структуру, на которую указывает statbuf, информацию, содержащуюся в(o) файле (его надо открыть), связанном с указателем filename. Структура stat определена в sys\stat.h. 
+++int stat (const char *pathname, struct stat *buf);

Lstat - похожа на stat, но возвращает информацию о символической ссылке, а не о файле(файл открывать не надо) на который та указывает. 
+++int lstat(const char *pathname, struct stat *buf);

Statx - Эта функция возвращает информацию о файле, сохраняя ее в буфер, на который указывает statxbuf. 
+++int statx(int dirfd, const char *pathname, int flags, unsigned int mask, struct statx *statxbuf);

