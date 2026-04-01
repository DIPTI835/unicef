<?php
session_start();
if (!isset($_SESSION["hospital_user"])) {
    header("Location: hospital_login.php");
    exit();
}
    
?>

<html>
<body>
<h2>Hospital Portal</h2>
<p>Welcome, <?php echo $_SESSION["hospital_user"]; ?>!</p>

<!-- link Cpp CGI -->
<a href="hospital.cgi">Go to Hospital Records</a>
<a href="hospital_logout.php">Logout</a>
</body>
</html>
