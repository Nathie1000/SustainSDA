#Sustain-wepapp

##Setup
First install NodeJS

After installing nodejs open a terminal (in administrator mode) inside the main project folder and type "npm install"
After it's done with installing, type inside the terminal "npm i nodemon -g"
Next go to the frontend/js map open a terminal (in administrator mode) and type "npm install"

To compile changes made to the frontend code make sure you run start_gulp.bat inside the frontend/js folder. This bat file will detect any changes made and compile it to 1 file.

###Database
Last but not least we need to setup the database. First we create a new database. Inside the file knex.json(in the main folder) we can change the details concerning the database, like name, username and password. After we've changed that we need to migrate all migration. To do that we open a terminal inside the main folder and we type: "npm install knex -g", to make sure we have knex installed. After that's installed we restart the terminal and type: "knex migrate:latest".

###Creating a first admin account
To make sure you can log in and can do stuff, you can create a standard admin accountant with the following credentials:
Username: "admin"
Password: "12345678"
To actually create it we open a terminal inside the main folder and run the command: "knex seed:run"


##Starting the server
To start the server go to the main project folder and run "start_test_server.bat", this will start the server locally
To connect to the server with your webbrowser go to http://www.localhost:3000

###When launching to production.
When you're going to launch the server in production make sure you change the environment to "production". We can do this in app.js inside the main folder. Search for the following line: "app.set('env', 'development');"
