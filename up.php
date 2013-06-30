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

print $json["id"];
print $json["stamp"];
print $json["light"];

print $json_str;

?>