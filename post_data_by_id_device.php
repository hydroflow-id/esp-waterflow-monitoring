<?php

include "config.php"; //berisi koneksi ke database
// $servername = "localhost";
// // REPLACE with your Database name
// $dbname = "esp_waterflow";
// // REPLACE with Database user
// $username = "root";
// // REPLACE with Database user password
// $password = "";
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

$api_key = $api_key_value = $id_device = $flow_rate = $volume = $status_valve = $level_battery = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Query untuk mencari API key di tabel user
    $sql_search = "SELECT id_device FROM user WHERE id_device = '$api_key'";
    $result = $conn->query($sql_search);
    
    $api_key = test_input($_POST["api_key"]);

    // Cek apakah data ditemukan
    if ($result->num_rows > 0) {
        // Jika data ditemukan, ambil data dan simpan ke variabel api_key_value
        $row = $result->fetch_assoc();
        $api_key_value = $row["id_device"];

        if($api_key == $api_key_value) {
            $id_device = test_input($_POST["id_device"]);
            $flow_rate = test_input($_POST["flow_rate"]);
            $volume = test_input($_POST["volume"]);
            $status_valve = test_input($_POST["status_valve"]);
            $level_battery = test_input($_POST["level_battery"]);
            
            // Check connection
            if ($conn->connect_error) {
                die("Connection failed: " . $conn->connect_error);
            } 
            
            $sql_post = "INSERT INTO water_consume (id_device, flow_rate, volume, status_valve, level_battery)
            VALUES ('" . $id_device . "', '" . $flow_rate . "', '" . $volume . "', '" . $status_valve . "', '" . $level_battery . "')";
            
            if ($conn->query($sql_post) === TRUE) {
                echo "New record created successfully";
            } 
            else {
                echo "Error: " . $sql_post . "<br>" . $conn->error;
            }
        
            $conn->close();
        }
        else {
            echo "Wrong API Key provided.";
        }

    }
}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}