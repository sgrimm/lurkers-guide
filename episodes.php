<?php

$domains = array(
  '.euro.net' => 'nl',
  '.pr.us' => 'pr',
  '.ar' => 'wbtv',
  '.at' => 'at',
  '.au' => 'au',
  '.au' => 'au',
  '.be' => 'be',
  '.br' => 'wbtv',
  '.ca' => 'ca',
  '.cl' => 'cl',
  '.co' => 'co',
  '.cr' => 'cr',
  '.cz' => 'cz',
  '.de' => 'de',
  '.es' => 'es',
  '.fi' => 'fi',
  '.fr' => 'fr',
  '.gb' => 'uk',
  '.gr' => 'gr',
  '.id' => 'id',
  '.ie' => 'uk',
  '.il' => 'il',
  '.it' => 'it',
  '.jo' => 'jo',
  '.my' => 'id',
  '.nl' => 'nl',
  '.no' => 'no',
  '.nz' => 'nz',
  '.pe' => 'pe',
  '.ph' => 'ph',
  '.pk' => 'pk',
  '.pl' => 'pl',
  '.pt' => 'pt',
  '.ru' => 'ru',
  '.se' => 'se',
  '.sg' => 'sg',
  '.si' => 'si',
  '.uk' => 'uk',
  '' => 'us',
);

$remote_host = gethostbyaddr($_SERVER['REMOTE_ADDR']);
$server_name = $_SERVER['SERVER_NAME'];

foreach ($domains as $suffix => $cnt) {
  if (!$suffix || substr($remote_host, -strlen($suffix)) == $suffix) {
    header("Location: http://$server_name/lurk/countries/$cnt/eplist.html");
    exit(0);
  }
}

print file_get_contents('internal/dom-error.html');
