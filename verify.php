<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $claim = escapeshellarg($_POST["claim"]);

  
   $output = shell_exec("\"C:\\Windows\\py.exe\" -3.13 C:\\xamp2\\cgi-bin\\chatbot_verify_who.py $claim"); #$output = shell_exec("\"C:\\Windows\\py.exe\" -3.13 C:\\xamp2\\cgi-bin\\chatbot_verify_who.py $claim");

    echo "<html><head><title>WHO Verification</title></head><body>";
    echo "<h1>Verification Result</h1>";

   
    echo "<pre>RAW OUTPUT:\n$output</pre>";

    $result = json_decode($output, true);

    if ($result && is_array($result)) {
        echo "<p>Status: " . htmlspecialchars($result["status"]) . "</p>";
        echo "<p>Confidence: " . htmlspecialchars($result["confidence"]) . "</p>";

        if (!empty($result["evidence"])) {
            echo "<h3>Evidence:</h3><ul>";
            foreach ($result["evidence"] as $e) {
                echo "<li><a href='" . htmlspecialchars($e["url"]) . "'>" . htmlspecialchars($e["title"]) . "</a> (score: " . $e["score"] . ")</li>";
            }
            echo "</ul>";
        } elseif (!empty($result["message"])) {
            echo "<p>" . htmlspecialchars($result["message"]) . "</p>";
        }
    } else {
        echo "<p>Error: Could not decode JSON.</p>";
    }

    echo "<br><a href='verify_form.php'>Back to Verification Form</a>";
    echo "</body></html>";
}
?>