<?php
session_start();
include("db.php");

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST["username"];
    $password = password_hash($_POST["password"], PASSWORD_DEFAULT);

    $sql = "INSERT INTO hospital_users (username, password) VALUES ('$username', '$password')";
    if ($conn->query($sql) === TRUE) {
        echo "Registration successful. <a href='hospital_login.php'>Login here</a>";
    } else {
        echo "Error: " . $conn->error;
    }
}
?>

<html>
<body>
<h2>Hospital Registration</h2>
<form method="post" action="hospital_register.php">
    Username: <input type="text" name="username" required><br>
    Password: <input type="password" name="password" required><br>
    <input type="submit" value="Register">
</form>
</body>
</html>