<?php
session_start();

if (isset($_POST['username']) && isset($_POST['password'])) {
    $server = "localhost";
    $dbuser = "root";
    $dbpass = "";
    $db = "vaccine_system";

    $con = mysqli_connect($server, $dbuser, $dbpass, $db);
    if (!$con) {
        die("Connection failed: " . mysqli_connect_error());
    }

    $username = $_POST["username"];
    $password = $_POST["password"];

    $sql = "SELECT * FROM manufacturers WHERE company_name='$username'";
    $result = mysqli_query($con, $sql);

    if (mysqli_num_rows($result) > 0) {
        $row = mysqli_fetch_assoc($result);

        if (password_verify($password, $row['password'])) {
            $_SESSION['company_name'] = $username;
            header("Location: vaccine_form.php");
            exit();
        } else {
            echo "Invalid password.";
        }
    } else {
        echo "Invalid username.";
    }

    mysqli_close($con);
}
?>

<form method="post">
    <input type="text" name="username" placeholder="Company Name" required>
    <input type="password" name="password" placeholder="Password" required>
    <button type="submit">Login</button>
</form>