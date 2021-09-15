# system-calls
# Second course

Stat - вносит в структуру, на которую указывает statbuf, информацию, содержащуюся в(o) файле (его надо открыть), связанном с указателем filename. Структура stat определена в sys\stat.h. 
+++int stat (const char *pathname, struct stat *buf);

Lstat - похожа на stat, но возвращает информацию о символической ссылке, а не о файле(файл открывать не надо) на который та указывает. 
+++int lstat(const char *pathname, struct stat *buf);

Statx - Эта функция возвращает информацию о файле, сохраняя ее в буфер, на который указывает statxbuf. 
+++int statx(int dirfd, const char *pathname, int flags, unsigned int mask, struct statx *statxbuf);

Open - открывает файл по указанному пути, если файла нет он может его создать. 
Фалги: O_RDONLY, O_WRONLY, O_RDWR, O_CREAT, O_TRUNC, O_EXCL
+++int open(const char *pathname, int flags);
+++int open(const char *pathname, int flags, mode_t mode); 0644

Write - записать в файловый дескриптор, какое-то количество байтов из буффера
+++ssize_t write(int fd, const void *buf, size_t count);
