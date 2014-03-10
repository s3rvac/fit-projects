{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

<div id="column2">
	<h2>{$L_MAIN_HEADER}</h2>
	<p>
		{$L_MAIN_DESCRIPTION|smarty:nodefaults}
	</p>

	<div class="align-center">
		<img src="{$TEMPLATES_PATH}images/main.png" alt="{$WEB_TITLE}" />
	</div>

	<br />

	<p>
		{$L_MAIN_CONTACT}: {$ADMINISTRATOR_EMAIL|smarty:nodefaults}
	</p>
</div>