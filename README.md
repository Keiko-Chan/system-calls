# Second course
# system-calls

Stat - вносит в структуру, на которую указывает statbuf, информацию, содержащуюся в файле, связанном с указателем filename. Структура stat определена в sys\stat.h. Структура stat определена в sys\stat.h. 
\\\\\\\\int stat (const char *pathname, struct stat *buf);

Lstat - похожа на stat, но возвращает информацию о символической ссылке, а не о файле на который та указывает. 
\\\\\\\\int lstat(const char *pathname, struct stat *buf);

fstat - идентичен stat(), но опрашиваемый файл задаётся в виде файлового дескриптора fd. 
\\\\\\\\int fstat(int fd, struct stat *buf);

fstatat -  Если в pathname задан относительный путь, то он считается относительно каталога, на который ссылается файловый дескриптор dirfd.
Если в pathname задан относительный путь и значение dirfd равно AT_FDCWD, то pathname рассматривается относительно текущего рабочего каталога вызывающего процесса (как stat()). 
\\\\\\\\int fstatat(int dirfd, const char *pathname, struct stat *buf, int flags);

Statx - Эта функция возвращает информацию о файле, сохраняя ее в буфер, на который указывает statxbuf. 
\\\\\\\\int statx(int dirfd, const char *pathname, int flags, unsigned int mask, struct statx *statxbuf);

Open - открывает файл по указанному пути, если файла нет он может его создать. 
Фалги: O_RDONLY, O_WRONLY, O_RDWR, O_CREAT, O_TRUNC, O_EXCL
\\\\\\\\int open(const char *pathname, int flags);
\\\\\\\\int open(const char *pathname, int flags, mode_t mode); 0644

Write - записать в файловый дескриптор, какое-то количество байтов из буффера.
\\\\\\\\ssize_t write(int fd, const void *buf, size_t count);

Close - закрывает файловый дескриптор, так что он больше не ссылается ни на один файл. 
\\\\\\\\int close(int fd);

Dprintf - записывает выходные данные в файловый дескриптор fd. 
\\\\\\\\int dprintf(int fd, const char *format, ...);

Read - читывает count байт из файла, описываемого аргументом fd, в буфер, на который указывает аргумент buf Указателю положения в файле дается приращение на количество считанных байт. Функция возвращает количество действительно считанных байт. Это количество может быть меньше, чем count, если встретился маркер конца файла или если до того, как было считано count байт, произошла ошибка. В случае ошибки возвращается —1, а 0 возвращается при попытке чтения в конце файла. 
\\\\\\\\int read(int fd, void *buf, unsigned count)

Pwrite - записывает максимум count байтов из буфера buf в файловый дескриптор fd , начиная со смещения offset. Текущая позиция файла не изменяется. 
\\\\\\\\ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset); 

Pread - читает максимум count байтов из файлового дескриптора fd, начиная со смещения offset (от начала файла), в буфер, начиная с buf. Текущая позиция файла не изменяется. 
\\\\\\\\ssize_t pread(int fd, void *buf, size_t count, off_t offset); 

fchmod - изменяет права доступа к файлу, на который указывает открытый файловый дескриптор fd. 
\\\\\\\\int fchmod(int fd, mode_t mode);

futimens - обновляет временные метки файла с наносекундной точностью. Файл указывается в виде открытого файлового дескриптора в fd. 
\\\\\\\\int futimens(int fd, const struct timespec times[2]);
