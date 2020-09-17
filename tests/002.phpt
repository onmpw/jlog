--TEST--
Check for jlog_info()
--SKIPIF--
<?php if (!extension_loaded("jlog")) print "skip"; ?>
--FILE--
<?php 
jlog_start();

$data = "王者";

var_dump(jlog_info("/tmp/jlog_test.txt",$data));

jlog_stop();
?>
--EXPECT--
bool(true)
