/*
Given a string: "Stuff happens." More stuff happens. "Wait --- ". "No, I do not think so."
-Replace all double-quotes that occur at the beginning with two left-quotes
-Replace all double-quotes that occur after spaces
--but only if said spaces are not preceded by em-dashes
-Replace left double-quote, single left-quote scheme by thinspace macro
*/

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define TEX_DIR "./scenes/"

void fixQuotes(char *str_ptr, char *old_str){
// Declare variable to store string with fixed quotes
    char new_str[1024]={0};
// New index for new string, plus variables to track em-dashes
    int j=0,dash_count=0,after_dash=0;
    for (int i=0; i<strlen(old_str); i++){
        if (old_str[i] == '"'){
            if (i == 0 || dash_count != 3 && old_str[i-1] == ' '){
// Two scenarios explained above
                new_str[j++]='`';
                new_str[j++]='`';
            }
            else {
// Accumulate double-quote normally otherwise
                new_str[j++]='"';
            }
        }
        else {
// Accumulate char's normally
            if (old_str[i] == '`' && old_str[i-1] == '"'){
                new_str[j++]='\\';
                new_str[j++]=',';
                new_str[j++]='`';
            }
            else {
                new_str[j++]=old_str[i];
            }
            if (old_str[i] == '-'){
                dash_count++;
            }
            else {
// Increment only after three consecutive dashes
                if (dash_count == 3){
                    after_dash++;
                }
/*
If fewer than three consecutive dashes are encountered (e.g. hyphenated words and stutters), then reset dash counters
*/
                else {
                    dash_count=0;
                    after_dash=0;
                }
                if (after_dash == 2){
// Reset dash counters two characters after em-dash
                    dash_count=0;
                    after_dash=0;
                }
            }
        }
    }
// Copy new string to pointer variable
    strcpy(str_ptr,new_str);
}

void changeExt(char *wpath){
// Manually changing the extension of a string because I don't know how to use string methods
    wpath[strlen(wpath)-2]='e';
    wpath[strlen(wpath)-1]='x';
}

void rewriteFile(char *dest){
// For fixQuotes call
    char old_str[1024],str_ptr[1024];
// Write destination
    char wpath[32]=TEX_DIR;
// Concatenate filename to write-directory
    strcat(wpath,strrchr(dest,'/')+1);
    changeExt(wpath);
    FILE *fp,*fq;
    fp=fopen(dest,"r");
    fq=fopen(wpath,"w");
// Process string and write string here
    while (fgets(old_str,sizeof(old_str),fp) != NULL){
        fixQuotes(str_ptr,old_str);
        fputs(str_ptr,fq);
    }
    fclose(fp);
    fclose(fq);
    return;
}

int main(int argc, char **argv){
    mkdir(TEX_DIR,S_IRWXU);
// Change every file in the argument list
    for (int i=1; i < argc; i++){
        rewriteFile(argv[i]);
    }
    return 0;
}
