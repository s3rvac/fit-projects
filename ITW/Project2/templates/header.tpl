{*
	Encoding: utf8
	Project:  ITW (2nd project)
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="cs" lang="cs">
<head>
	<meta http-equiv="content-type" content="text/html; charset=utf-8" />
	<meta http-equiv="content-language" content="cs" />
	<meta name="author" content="Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/" />
	<meta name="copyright" content="Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/" />
	<meta name="description" content="Postapokalyptický park Yooleg - překrásný přírodní park na Brněnské planině obsahující to, co zbylo..." />
	<meta name="keywords" content="Yooleg, apokalypsa, park, příroda, budoucnost, Brno, Brněnská planina, Veveří" />
	<meta name="robots" content="index,follow" />
	<meta name="googlebot" content="index,follow,snippet,archive" />
	<link rel="icon" type="image/gif" href="./images/favicon.gif" />
	<link rel="shortcut icon" type="image/gif" href="./images/favicon.gif" />
	<link id="user_style" rel="stylesheet" type="text/css" media="screen,projection" href="./styles/style1.css" />
	<link rel="stylesheet" type="text/css" media="print" href="./styles/print.css" />
	<script type="text/javascript" src="./scripts/functions.js"></script>
	<script type="text/javascript">useUsersStyle('./styles/');</script>
	<title>Postapokalyptický park Yooleg {if $PAGE_TITLE neq ''}- {$PAGE_TITLE}{/if}</title>
</head>
<body onload="init();">
	<div id="page">
		<div id="logo"></div>
		<h1><a href="./index.php" title="Přejít na úvodní stránku"><span>Postapokalyptický park Yooleg</span></a></h1>

		<div id="greeting"></div>

		<div id="location">
			Navigace: <a href="./index.php?page=index" title="Přejít na úvodní stránku">Yooleg</a>
			{if $PAGE neq ''}
				{if $PAGE eq 'about_park'}
					> <a href="./index.php?page=about_park" title="Přejít na informace o parku">O parku</a>
				{elseif $PAGE eq 'history'}
					> <a href="./index.php?page=about_park" title="Přejít na informace o parku">O parku</a>
					> <a href="./index.php?page=history" title="Přejít na historii parku">Historie parku</a>
				{elseif $PAGE eq 'future'}
					> <a href="./index.php?page=about_park" title="Přejít na informace o parku">O parku</a>
					> <a href="./index.php?page=future" title="Přejít na budoucnost parku">Budoucnost parku</a>
				{elseif $PAGE eq 'park'}
					> <a href="./index.php?page=park" title="Přejít na místa v parku">Kam se podívat</a>
				{elseif $PAGE eq 'veveri'}
					> <a href="./index.php?page=park" title="Přejít na místa v parku">Kam se podívat</a>
					> <a href="./index.php?page=veveri" title="Přejít na trosky hradu Veveří">Trosky hradu Veveří</a>
				{elseif $PAGE eq 'dam'}
					> <a href="./index.php?page=park" title="Přejít na místa v parku">Kam se podívat</a>
					> <a href="./index.php?page=dam" title="Přejít na vyprahlou přehradu">Vyprahlá přehrada</a>
				{elseif $PAGE eq 'fauna_flora'}
					> <a href="./index.php?page=attractions" title="Přejít na živočichy a rostliny">Živočichové a rostliny</a>
				{elseif $PAGE eq 'animals'}
					> <a href="./index.php?page=attractions" title="Přejít na živočichy a rostliny">Živočichové a rostliny</a>
					> <a href="./index.php?page=animals" title="Přejít na zajímavosti o živočiších">Živočichové</a>
				{elseif $PAGE eq 'plants'}
					> <a href="./index.php?page=attractions" title="Přejít na živočichy a rostliny">Živočichové a rostliny</a>
					> <a href="./index.php?page=plants" title="Přejít na zajímavosti o rostlinách">Rostliny</a>
				{elseif $PAGE eq 'contact'}
					> <a href="./index.php?page=contact" title="Přejít na kontakt">Kontakt</a>
				{elseif $PAGE eq 'web_map'}
					> <a href="./index.php?page=web_map" title="Přejít na mapu webu">Mapa webu</a>
				{elseif $PAGE eq 'links'}
					> <a href="./index.php?page=links" title="Přejít na odkazy">Odkazy</a>
				{elseif $PAGE eq 'personalization'}
					> <a href="./index.php?page=personalization" title="Přejít na personalizaci">Personalizace</a>
				{elseif $PAGE eq 'factors'}
					> <a href="./index.php?page=factors" title="Přejít na přehled faktorů, přístupnost a validaci">Faktory, přístupnost a validace</a>
				{/if}
			{/if}
		</div>

		<hr />

		<div id="menu">
			<h2>Menu</h2>
			<ul>
				<li>
					<a href="./index.php?page=index" title="Přejít na úvod">Úvod</a>
				</li>
				<li>
					<a href="./index.php?page=about_park" title="Přejít na informace o parku">O parku</a>
					{if $PAGE eq 'about_park' or $PAGE eq 'history' or $PAGE eq 'future'}
					<ul>
						<li><a href="./index.php?page=history" title="Přejít na historii parku">Historie parku</a></li>
						<li><a href="./index.php?page=future" title="Přejít na budoucnost parku">Budoucnost parku</a></li>
					</ul>
					{/if}
				</li>
				<li>
					<a href="./index.php?page=park" title="Přejít na mapu parku">Kam se podívat</a>
					{if $PAGE eq 'park' or $PAGE eq 'veveri' or $PAGE eq 'dam'}
					<ul>
						<li><a href="./index.php?page=veveri" title="Přejít na trosky hradu Veveří">Trosky hradu Veveří</a></li>
						<li><a href="./index.php?page=dam" title="Přejít na vyprahlou přehradu">Vyprahlá přehrada</a></li>
					</ul>
					{/if}
				</li>
				<li>
					<a href="./index.php?page=fauna_flora" title="Přejít na živočichy a rostliny">Živočichové a rostliny</a>
					{if $PAGE eq 'fauna_flora' or $PAGE eq 'animals' or $PAGE eq 'plants'}
					<ul>
						<li><a href="./index.php?page=animals" title="Přejít na zajímavosti o živočiších">Živočichové</a></li>
						<li><a href="./index.php?page=plants" title="Přejít na zajímavosti o rostlinách">Rostliny</a></li>
					</ul>
					{/if}
				</li>
				<li>
					<a href="./index.php?page=contact" title="Přejít na kontakt">Kontakt</a>
				</li>
				<li>
					<a href="./index.php?page=links" title="Přejít na odkazy">Odkazy</a>
				</li>
				<li>
					<a href="./index.php?page=personalization" title="Přejít na personalizaci">Personalizace</a>
				</li>
			</ul>
		</div>

		<hr />

		<div id="content">