#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <filesystem>

using namespace std;

bool check_permis(const char *path,string file_name) {

    struct stat st;
    if (stat(path, &st) != 0) {
        perror("Not able to get stat path of given file");
        return false;
    }

    mode_t perms = st.st_mode;
    if(perms & S_IRUSR){
        cout<<"User read permissions on "<<file_name<<": Yes"<<endl;
    }else cout<<"User read permissions on "<<file_name<<": No"<<endl;

    if(perms & S_IWUSR){
        cout<<"User has write permission on "<<file_name<<": Yes"<<endl;
    }else cout<<"User has write permission on "<<file_name<<": No"<<endl;

    if(perms & S_IXUSR){
        cout<<"User has execute permission on "<<file_name<<": Yes"<<endl;
    }else cout<<"User has execute permission on "<<file_name<<": No"<<endl;

    if(perms & S_IRGRP){
        cout<<"Group has read permissions on "<<file_name<<": Yes"<<endl;
    }else cout<<"Group has read permissions on "<<file_name<<": No"<<endl;

    if(perms & S_IWGRP){
        cout<<"Group has write permission on "<<file_name<<": Yes"<<endl;
    }else cout<<"Group has write permission on "<<file_name<<": No"<<endl;

    if(perms & S_IXGRP){
        cout<<"Group has execute permission on "<<file_name<<": Yes"<<endl;
    }else cout<<"Group has execute permission on "<<file_name<<": No"<<endl;

    if(perms & S_IROTH){
        cout<<"Others has read permissions on "<<file_name<<": Yes"<<endl;
    }else cout<<"Others has read permissions on "<<file_name<<": No"<<endl;

    if(perms & S_IWOTH){
        cout<<"Others has write permissions on "<<file_name<<": Yes"<<endl;
    }else cout<<"Others has write permissions on "<<file_name<<": No"<<endl;

    if(perms & S_IXOTH){
        cout<<"Others has execute permissions on "<<file_name<<": Yes"<<endl;
    }else cout<<"Others has execute permissions on "<<file_name<<": No"<<endl;

    cout<<'\n';
    return true;
}

bool reverseStore(const char *newfile, const char *oldfile) {
    int fd_new = open(newfile, O_RDONLY);
    int fd_old = open(oldfile, O_RDONLY);

    if (fd_new < 0 || fd_old < 0) {
        perror("Not able to open files");
        return false;
    }

    long long new_size = lseek(fd_new, 0, SEEK_END);
    long long old_size = lseek(fd_old, 0, SEEK_END);

    long long new_size1 = new_size;

    lseek(fd_new, 0, SEEK_SET);
    lseek(fd_old, 0, SEEK_SET);

    const int buf_size = 10000;
    char new_buf[buf_size], old_buf[buf_size];

    while (new_size1 > 0) {
        long long to_read;

        if(new_size1 > buf_size){
            to_read = buf_size;
        }else to_read = new_size1;

        lseek(fd_new, new_size1 - to_read, SEEK_SET);
        read(fd_new, new_buf, to_read);        
        read(fd_old, old_buf, to_read);

        for (long long i = 0; i < to_read; ++i) {
            if (new_buf[i] != old_buf[to_read - 1 - i]) {
                cout<<"Whether file contents are reversed in newfile: No" <<endl;
                return false;
            }
        }

        new_size1 -= to_read;
    }
    cout << "Whether file contents are reversed in newfile: Yes"<<endl;


    if (new_size != old_size) {
        cout<<"Both Files Sizes are Same : No"<<endl;
    }else cout<<"Both Files Sizes are Same : Yes"<<endl;

    cout<<'\n';
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        perror("Your execute format is not ./a.out <newfile_path> <oldfile_path> <directory_path>"); 
        return 1;
    }

    const char *newfile = argv[1];
    const char *oldfile = argv[2];
    const char *directory = argv[3];

    // Check for directory existence
    if(filesystem::exists(directory)){
        cout<<"Directory is created: Yes"<<endl;
    }else cout<<"Directory is created: No"<<endl;

    if(!(filesystem::exists(newfile))){
        cout<<"newfile does not exist: "<< newfile <<endl;
    }

    // Check if file contents are reversed
    reverseStore(newfile, oldfile);

    // Check permissions for the files and directory
    check_permis(newfile,"newfile");

    check_permis(oldfile,"oldfile");

    check_permis(directory,"directory");

    return 0;
}

