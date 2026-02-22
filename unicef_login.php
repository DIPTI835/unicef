<?php
session_start();
include("db1.php");

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST["username"];
    $password = $_POST["password"];

    $sql = "SELECT * FROM unicef_users WHERE username='$username'";
    $result = $conn->query($sql);

    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        if (password_verify($password, $row["password"])) {
            $_SESSION["unicef_user"] = $username;
            header("Location: unicef_portal.php");
            exit();
        } else {
            echo "Invalid password.";
        }
    } else {
        echo "No such user.";
    }
}
?>

<html>
<body>
<h2>Unicef Login</h2>
<form method="post" action="unicef_login.php">
    Username: <input type="text" name="username"><br>
    Password: <input type="password" name="password"><br>
    <input type="submit" value="Login">
</form>
</body>
</html>