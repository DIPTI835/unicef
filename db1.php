<?php
$server = "localhost";
$dbuser = "root";
$dbpass = "";
$dbname = "unicef";   

$conn = new mysqli($server, $dbuser, $dbpass, $dbname);





if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
echo "DEBUG: Using db1.php, database = $dbname<br>";
?>
