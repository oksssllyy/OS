#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <dlfcn.h>
int process(char *arg){
    int x = 0;
    if (!strcmp(arg, "-d"))
    {
        printf("x--\n");
        x--;
    }
    else if(!strcmp(arg, "-i"))
    {
        printf("x++\n");
        x++;
    }
    return x;
}
int new_client(int sock)
{
    FILE *ERRstat;
    char buf[255];
    int size;
    int newsock, clnlen;
    int x;
    struct sockaddr_in client;
    newsock = accept(sock, &client, &clnlen); // появление нового клиента
    char *error = "";
    void *handle = dlopen ("/home/kuro/labs/lab6/libcookies.so", RTLD_LAZY);
    if (!handle) {
        fputs (dlerror(), stderr);
        exit(-1);
    }

    printf("New client: %s\n",inet_ntoa(client.sin_addr));
    while((size = recv(newsock, buf, sizeof(buf), 0)) != 0)
    {
        x = -3;
        if (!strcmp(buf, "-d") || !strcmp(buf, "-i"))
        {
            x = process(buf);
        }


            void (*world)() = dlsym(handle, "cookie");
            if ((error = dlerror()) != NULL) {
                ERRstat = fopen("ERRORS.txt", "a+");
                fprintf (ERRstat, "%s\n %d", error);
                fclose(ERRstat);
                exit(-1);
            }
            (*world)();
            dlclose(handle);
        send(newsock, &x, sizeof(x), 0); // отправляем эхо
    } // пока получаем от клиента
    close(newsock);
    return 0;
}


int main(int argc, char *argv[])
{
    struct sockaddr_in server, client;
    int sock = socket(AF_INET, SOCK_STREAM, 0); // создание сокета

    // структура для сервера
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    server.sin_port = htons(2019); // порт сервера
    bind(sock, &server, sizeof(server)); // связка с сокетом
    listen(sock,5); // размер очереди
    pthread_t thread; //создаем идентификатор потока
    while(1)
    {
        pthread_create(&thread, NULL, new_client, sock);
        pthread_join(thread, NULL);

    }

    close(sock);

    //ждем завершения потока

    //создаем поток по идентификатору thread и функции потока threadFunc
    //и передаем потоку указатель на данные thread_data


}
