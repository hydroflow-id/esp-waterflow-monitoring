<?php
// Token auth
// $valid_token = '12345asdf';

// if (!isset($_GET['token']) || $_GET['token'] != $valid_token) {
//     header('HTTP/1.0 401 Unauthorized');
//     echo 'Authorization required';
//     exit;
// }


$valid_username = 'user';
$valid_password = '12345678';

if (!isset($_SERVER['PHP_AUTH_USER']) || !isset($_SERVER['PHP_AUTH_PW']) || $_SERVER['PHP_AUTH_USER'] !== $valid_username || $_SERVER['PHP_AUTH_PW'] !== $valid_password) {
    header('HTTP/1.0 401 Unauthorized');
    header('WWW-Authenticate: Basic realm="My Protected Area"');
    echo 'Authorization required';
    exit;
}

// Rest of your code for fetching data and sending JSON response
include "config.php";

$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

header('Content-Type: text/event-stream');
header('Cache-Control: no-cache');
header('Connection: keep-alive');

function sendUpdate($data) {
    echo "data: " . json_encode($data) . "\n\n";
    ob_flush();
    flush();
}

$lastData = array();

while (true) {
    $sql = "SELECT id, value1, value2, value3, value4, reading_time FROM espdata ORDER BY reading_time DESC LIMIT 40";
    $result = $conn->query($sql);

    $data = array();
    while ($row = $result->fetch_assoc()) {
        $data[] = $row;
    }

    if ($data !== $lastData) {
        sendUpdate($data);
        $lastData = $data;
    }

    // Berikan waktu singkat untuk CPU beristirahat sebelum melakukan loop kembali
    usleep(100000);
}
?>
