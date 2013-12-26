<?php

$domains = array(
  '.euro.net' => 'countries/nl/eplist.html',
  '.pr.us' => 'countries/pr/eplist.html',
  '.ar' => 'countries/wbtv/eplist.html',
  '.at' => 'countries/at/eplist.html',
  '.au' => 'countries/au/eplist.html',
  '.au' => 'countries/au/eplist.html',
  '.be' => 'countries/be/eplist.html',
  '.br' => 'countries/wbtv/eplist.html',
  '.ca' => 'countries/ca/eplist.html',
  '.cl' => 'countries/cl/eplist.html',
  '.co' => 'countries/co/eplist.html',
  '.cr' => 'countries/cr/eplist.html',
  '.cz' => 'countries/cz/eplist.html',
  '.de' => 'countries/de/eplist.html',
  '.es' => 'countries/es/eplist.html',
  '.fi' => 'countries/fi/eplist.html',
  '.fr' => 'countries/fr/eplist.html',
  '.gb' => 'countries/uk/eplist.html',
  '.gr' => 'countries/gr/eplist.html',
  '.id' => 'countries/id/eplist.html',
  '.ie' => 'countries/uk/eplist.html',
  '.il' => 'countries/il/eplist.html',
  '.it' => 'countries/it/eplist.html',
  '.jo' => 'countries/jo/eplist.html',
  '.my' => 'countries/id/eplist.html',
  '.nl' => 'countries/nl/eplist.html',
  '.no' => 'countries/no/eplist.html',
  '.nz' => 'countries/nz/eplist.html',
  '.pe' => 'countries/pe/eplist.html',
  '.ph' => 'countries/ph/eplist.html',
  '.pk' => 'countries/pk/eplist.html',
  '.pl' => 'countries/pl/eplist.html',
  '.pt' => 'countries/pt/eplist.html',
  '.ru' => 'countries/ru/eplist.html',
  '.se' => 'countries/se/eplist.html',
  '.sg' => 'countries/sg/eplist.html',
  '.si' => 'countries/si/eplist.html',
  '.uk' => 'countries/uk/eplist.html',
  '' => 'countries/us/eplist.html',
);

$remote_host = gethostbyaddr($_SERVER['REMOTE_ADDR']);
$server_name = $_SERVER['SERVER_NAME'];

foreach ($domains as $suffix => $url) {
	if (!$suffix || substr($remote_host, -strlen($suffix)) == $suffix) {
		header("Location: http://$server_name/lurk/$url");
	}
}

print file_get_contents('internal/dom-error.html');
