<?php
if (isset($_POST['company_name']) && isset($_POST['password'])) {
    $server = "localhost";
    $dbuser = "root";
    $dbpass = "";
    $dbname = "vaccine_system";

    $con = mysqli_connect($server, $dbuser, $dbpass, $dbname);
    if (!$con) {
        die("Connection failed: " . mysqli_connect_error());
    }

    $company_name = $_POST["company_name"];
    $password = $_POST["password"];

    $hashed_password = password_hash($password, PASSWORD_DEFAULT);

    $sql = "INSERT INTO manufacturers (company_name, password) VALUES ('$company_name', '$hashed_password')";
    if (mysqli_query($con, $sql)) {
        echo "Registration successful!";
    } else {
        echo "Error: " . mysqli_error($con);
    }

    mysqli_close($con);
}
?>
<form method="post">
    <input type="text" name="company_name" placeholder="Company Name" required>
    <input type="password" name="password" placeholder="Password" required>
    <button type="submit">Register</button>
</form>