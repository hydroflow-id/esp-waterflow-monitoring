# waterflow-monitoring
Visualize waterflow usage from Anywhere in the World (ESP32 + MySQL + PHP) 
in real time with **Server-Sent Events (SSE)**, which allows servers to send data in real time to clients via the HTTP protocol.

## Install on localhost
Use git clone https://github.com/arifnurrizqi/esp-waterflow-monitoring.git or Download this repository in zip form

then follow these steps:
1. Install xampp
2. Run & start apache + mysql
3. Copy and extract the script file in the xampp/htdocs folder
4. Create database in PhpMyAdmin : localhost/phpmyadmin
Example here: esp_waterflow
5. Open the extracted script folder in xampp/htdocs
6. Open the config.php file
8. Change the database name, the same as the new database name in step 4:

Example here: esp_waterflow
```
$servername = "localhost";
$dbname = "esp_waterflow";
$username = "root";
$password = "";
```

9. Import the web database to phpmyadmin
10. After the database is imported. The web is ready to run on localhost with the address: localhost/NAMEFOLDER
example here: localhost/esp-waterflow-monitoring
11. Done.

### default username & password
```
username : user
password : 12345678
```
## Demo
<a href="https://iot.bem-unwiku.com"> For Demo Click Here</a>
