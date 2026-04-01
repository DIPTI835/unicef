<?php
// unicef_portal.php
session_start();
if (!isset($_SESSION["unicef_user"])) {
    header("Location: unicef_login.php");
    exit();
}
    
?>

<html>
<body>
<h2>Unicef Portal</h2>
<p>Welcome, <?php echo $_SESSION["unicef_user"]; ?>!</p>

<!-- link to  Cpp CGI -->
<a href="/cgi-bin/unicef.exe">Go to Unicef Records</a>
<a href="unicef_logout.php">Logout</a>
</body>
</html>


<h2>Ask Any Question (Parent Chatbot)</h2>
<form method="post" action="verify.php">
    Enter your claim: <input type="text" name="claim" size="80"><br>
    <input type="submit" value="Check with WHO">
</form>
