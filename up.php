<html>
<head>
<title>Upload page</title>
</head>
<body>

<p>Here's the php</p>

<?php

print "_POST:<br/>";
var_dump($_POST);

$json_get = $_GET["json"];
$json_post = $_POST["json"];
$json_str = $json_get;

if ($json_post)
{
 $json_str = $json_post;
}

$json = json_decode($json_str,true);

print "json_str:<br/>";
print $json_str;

print "<br/><br/>decoded json object:<br/>";
var_dump($json);

$dangerous = array(".", "\\", "!", "\/");
$json["id"] = str_replace($dangerous, "", $json["id"]);

if (($json != NULL) and ($json["id"] != NULL)) {
 file_put_contents($json["id"] . ".json", $json_str);
}

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
