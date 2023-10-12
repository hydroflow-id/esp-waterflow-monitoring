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

function debit(){
    global $conn;
    $query = "SELECT value1 FROM espdata ORDER BY reading_time DESC LIMIT 1";
    return $conn->query($query);
}
function usage(){
    global $conn;
    $query = "SELECT value2 FROM espdata ORDER BY reading_time DESC LIMIT 1";
    return $conn->query($query);
}
function data(){
    global $conn;
    $query = "SELECT id, value1, value2, reading_time FROM espdata ORDER BY reading_time DESC LIMIT 40";
    return $conn->query($query);
}

while (true) {
    // Memanggil fungsi debit()
    $debitResult = debit();
    $debitValue = $debitResult->fetch_assoc();

    // Memanggil fungsi usage()
    $usageResult = usage();
    $usageValue = $usageResult->fetch_assoc();

    // Memanggil fungsi data()
    $dataResult = data();
    $dataArray = array(); // Array untuk menyimpan data

    // Memasukkan hasil debit dan usage ke dalam array
    $dataArray['debit'] = $debitValue;
    $dataArray['usage'] = $usageValue;

    // Memasukkan hasil data ke dalam array
    while ($row = $dataResult->fetch_assoc()) {
        $dataArray['data'][] = $row;
    }

    if ($dataArray !== $lastData) {
        sendUpdate($dataArray);
        $lastData = $dataArray;
    }

    // Berikan waktu singkat untuk CPU beristirahat sebelum melakukan loop kembali
    usleep(100000);
}
?>
