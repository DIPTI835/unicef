<?php
session_start();
include("db.php");
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST["username"];
    $password = $_POST["password"];

    $sql = "SELECT * FROM hospital_users WHERE username='$username'";
    $result = $conn->query($sql);
if ($result && $result->num_rows > 0) {
        $row = $result->fetch_assoc();
        if (password_verify($password, $row["password"])) {
            $_SESSION["hospital_user"] = $username;
            header("Location: hospital_portal.php");
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
<h2>Hospital Login</h2>
<form method="post" action="hospital_login.php">
    Username: <input type="text" name="username" required><br>
    Password: <input type="password" name="password" required><br>
    <input type="submit" value="Login">
</form>
</body>
</html>