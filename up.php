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
print 'Last error: ', $json_errors[json_last_error()], PHP_EOL, PHP_EOL;

print "json_str:\n";
print $json_str;

print "decoded json object:\n";
var_dump($json);

print "decoded id:\n";
print $json["id"];
print "decoded stamp:\n";
print $json["stamp"];
print "decoded light:\n";
print $json["light"];

?>

</body>
