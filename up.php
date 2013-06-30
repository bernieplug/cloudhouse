<html>
<head>
<title>Upload page</title>
</head>
<body>

<p>Here's the php</p>

<?php

$json_get = $_GET["json"];
$json_post = $_POST["json"];
$json = $json_get;

if ($json_post)
{
 $json = $json_post;
}

if ((strlen($json["id"] > 5) and (strlen($json["id") < 16)) {
  print("has an id");
}

var_dump(json_decode($json));

phpinfo();

?>