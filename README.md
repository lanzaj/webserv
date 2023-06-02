# Fonctions deja connues 
- execve 
- dup
- dup2
- pipe
- strerror
- gai_strerror
- errno
- dup
- dup2
- fork



# Nouvelles fonctions

## htons, htonl, ntohs, ntohl
The htonl() function converts the unsigned integer hostlong from host byte order to network byte order.
The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
The ntohl() function converts the unsigned integer netlong from network byte order to host byte order.
The ntohs() function converts the unsigned short integer netshort from network byte order to host byte order.

*En gros ca permet de gerer les problemes de endian, c'est a dire la facon dont les nombres (int, short etc.) sont stockes par le system. En gros le principe est de s'assurer que peut importe la facon dont les donnes sont organises dans la machine(bigendian ou shortendian), elles sont envoyes dans une norme ok pour le reseau.*

Une video pour comprendre l'interet de ces fonctions :
https://youtu.be/OoHich9BPxg

*Je kiffe ce mec ! D'apres lui pour un serveur HTTP on devrait pas trop avoir besoin de se faire chier avec ces fonctions car on devrait envoyer du texte et pas des int ou des short etc.*

## select
Petite explication toujours via mon youtuber chauve prefere : 
https://youtu.be/Y6pFtgRdUts
C'est un moyen d'avoir un peu de parallélisation sans pour autant utiliser des threads. 
Prend un groupe de File Descriptor et te dis quand il y a quelque chose a lire sur l'un de ces FD. 

**Pas sur que ca soit vraiment interessant d'utiliser a la place des Threads, mais a voir...**

## poll

## epoll (epoll_create, epoll_ctl, epoll_wait)

## kqueue (kqueue, kevent)

## socket

## accept

## listen

## send

## recv

## bind

## connect

## getaddrinfo

## freeaddrinfo

## setsockopt

## getsockname

## getprotobyname

## fcntl



# Ressources interessantes
## Understanding Nginx Server and Location Block Selection Algorithms
https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms

# Write Your Own HTTP Server
https://betterprogramming.pub/writing-your-own-http-server-introduction-b2f94581268b

# Documentation officielle HTTP
https://www.rfc-editor.org/rfc/rfc9112.html

# Wikipedia Common Gateway Interface
https://en.wikipedia.org/wiki/Common_Gateway_Interface

# Wikipedia Werbserver
https://en.wikipedia.org/wiki/Web_server

# Lecture universite sur Poll et Select 
https://youtu.be/tAGF0T2cXRA
