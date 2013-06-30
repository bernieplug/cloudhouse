<html>
<head>
<title>Upload page</title>
</head>
<body>

<p>Here's the php</p>

<?php

$json_get = $_GET["json"];
$json_post = $_POST["json"];
$json_str = $json_get;

if ($json_post)
{
 $json_str = $json_post;
}

$json = json_decode($json_str);

print "json_str:\n";
print $json_str;

print "<br/><br/>decoded json object:\n";
var_dump($json);

print "<br/><br/>last error:<br/>";
print $json_errors[json_last_error()];

print "<br/><br/>decoded id:<br/>";
print $json["id"];
print "<br/><br/>decoded stamp:<br/>";
print $json["stamp"];
print "<br/><br/>decoded light:<br/>";
print $json["light"];

?>

</body>
