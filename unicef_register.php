<?php
session_start();
include("db1.php"); // ✅ connects to unicef DB
echo "Connected to database: " . $dbname;

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST["username"];
    $password = password_hash($_POST["password"], PASSWORD_DEFAULT);

    $sql = "INSERT INTO unicef_users (username, password) VALUES ('$username', '$password')";
    if ($conn->query($sql) === TRUE) {
        echo "Registration successful. <a href='unicef_login.php'>Login here</a>";
    } else {
        echo "Error: " . $conn->error;
    }
}
?>
<html>
<body>
<h2>Unicef Registration</h2>
<form method="post" action="unicef_register.php">
    Username: <input type="text" name="username"><br>
    Password: <input type="password" name="password"><br>
    <input type="submit" value="Register">
</form>
</body>
</html>