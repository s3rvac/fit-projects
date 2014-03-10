{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}
{if $BROWSER_TYPE eq 'IE'}
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="{$USER_LANGUAGE}" lang="{$USER_LANGUAGE}">
{else}
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="{$USER_LANGUAGE}">
{/if}
<head>
	<meta http-equiv="content-language" content="{$USER_LANGUAGE}" />
	{if $BROWSER_TYPE eq 'IE'}
	<meta http-equiv="content-type" content="text/html; charset=utf-8" />
	{else}
	<meta http-equiv="content-type" content="application/xhtml+xml; charset=utf-8" />
	{/if}
	<meta name="author" content="Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/" />
	<meta name="copyright" content="Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/" />
	<meta name="description" content="{$WEB_DESCRIPTION}" />
	<meta name="keywords" content="{$WEB_KEYWORDS}" />
	<link rel="icon" type="image/gif" href="{$TEMPLATES_PATH}icons/favicon.gif" />
	<link rel="shortcut icon" type="image/gif" href="{$TEMPLATES_PATH}icons/favicon.gif" />
	<link rel="stylesheet" type="text/css" media="screen,projection" href="{$TEMPLATES_PATH}styles/screen.css" />
	<link rel="stylesheet" type="text/css" media="print" href="{$TEMPLATES_PATH}styles/print.css" />
	<title>{$WEB_TITLE}</title>
</head>
<body>
	<div id="main">
		<div id="header">
			<table width="97%" border="0">
				<tr>
					<td id="header-left">
						<h1 id="web-title">{$WEB_TITLE}</h1>
					</td>
					<td id="header-right">
						<div id="language">
							{foreach item=language from=$AVAILABLE_LANGUAGES}
								{foreach key=short_name item=url from=$language}
									<a href="{$url}"><img src="{$TEMPLATES_PATH}images/languages/{$short_name}.gif" alt="{$short_name}" /></a>
								{/foreach}
							{/foreach}
						</div>
						<div id="login">
						{if $IS_LOGGED}
							<strong>{$L_LOGGED}:</strong> {$LOGGED_USERNAME}
							<table border="0">
								<tr>
									<td>
										<form action="{$U_PROFILE}" method="post">
											<div>
												<input class="submit" type="submit" name="profile" value="{$L_PROFILE}" />
											</div>
										</form>
									</td>
									<td>
										<form action="{$U_LOGOUT}" method="post">
											<div>
												<input class="submit" type="submit" name="logout" value="{$L_LOGOUT}" />
											</div>
										</form>
									</td>
								</tr>
							</table>
						{else}
							<div>
								<form action="{$U_LOGIN}" method="post">
									<div style="margin-bottom: 2px;">
										<strong>{$L_USERNAME}:&nbsp;&nbsp;</strong><input type="text" class="text" size="16" name="login_username" value="{$LOGIN_USERNAME}" />
									</div>
									<div>
										<strong>{$L_PASSWORD}:&nbsp;&nbsp;</strong><input type="password" class="text" size="16" name="login_password" value="" />
									</div>
									<div class="message">
										{if $AUTH_ERROR}
											<span class="error-message">{$AUTH_ERROR_MESSAGE}</span>
										{elseif $AUTH_SUCCESS}
											<span class="success-message">{$AUTH_SUCCESS_MESSAGE}</span>
										{/if}
										<input class="submit" type="submit" name="login" value="{$L_LOGIN}" />
									</div>
								</form>
							</div>
						{/if}
						</div>
					</td>
				</tr>
			</table>
		</div>
		<div id="content">
