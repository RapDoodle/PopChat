# PopChat

## Introduction
PopChat is an open-source online chatting software licensed under the General Public License v3 (GPLv3) and implements the client/server networking protocol based on the application layer protocol POP/1.0. It supports group chat on any computer that runs an instance of PopChat Client that has access to a PopChat server. 

<img src="https://raw.githubusercontent.com/RapDoodle/PopChat/master/docs/screenshots/01.jpg" height="350"/><br>
<img src="https://raw.githubusercontent.com/RapDoodle/PopChat/master/docs/screenshots/02.jpg" height="350"/>

## Frameworks
- The network communication is implemented on top of Windows Socket 2 
- The graphical user interface for the client is implemented on top of Qt, an open-source framework.

## Instructions to Setup a PopChat Server
1. Install MySQL Server. For more information, visit https://www.mysql.com/. Or download XAMPP and start the MySQL server from there. For more information about XAMPP, visit https://www.apachefriends.org/index.html.
1. Create a database called `pop`. For more instructions visit https://www.mysql.com/.
    ```SQL
    CREATE DATABASE pop;
    ```
1. Open cmd or PowerShell, `cd` to the directory of the executable. Specify the parameters
    ~~~shell
    server -p [port number] --dbu [username] --dbp [password] --dbport [db port number]
    ~~~
    By default, the server will run on port `8000`, the username for the database is `test`, the password for the database is empty, and the default database port number defaults to MySQL's default port number `3306`. If you start your MySQL server using XAMPP and made no modification to the database's profile, for command prompt, to run the server on port 1234
    ~~~shell
    server -p 1234 
    ~~~
    In PowerShell, type
    ~~~shell
    .\server -p 1234
    ~~~
