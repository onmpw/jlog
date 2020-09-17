--TEST--
Check for jlog_error()
--SKIPIF--
<?php if (!extension_loaded("jlog")) print "skip"; ?>
--FILE--
<?php 
jlog_start();

$data = "王者荣耀";

var_dump(jlog_error("/tmp/jlog_test.txt",$data));

jlog_stop();
?>
--EXPECT--
bool(true)
