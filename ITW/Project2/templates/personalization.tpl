{*
	Encoding: utf8
	Project:  ITW (2nd project)
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}
<h2>Personalizace</h2>

<p>
	Zde si můžete upravit vzhled webu podle podle své nálady.
</p>

<form id="change_style" action="./index.php?page=personalization" onsubmit="changeUsersStyle(this);">
	<fieldset>
		<legend>Změna stylu</legend>
		<input type="radio" name="styles" value="none" />Bez stylu<br />
		<input type="radio" name="styles" value="style1" />Tmavý<br />
		<input type="radio" name="styles" value="style2" />Světlý<br />
		<input type="submit" class="button" value="Změnit styl" />
	</fieldset>
</form>

<form id="change_greeting" action="./index.php?page=personalization" onsubmit="changeGreeting(this.greeting.value);">
	<fieldset>
		<legend>Změnit pozdrav</legend>
		<p>Pokud si přejete, aby Vás náš web vítal oslovením, vyplňte zvolené oslovení do řádku níže (např. "ty, který jsi přežil").</p> <br />
		<input class="text_input" type="text" name="greeting" size="30" value="" />
		<input type="submit" class="button" value="Změnit pozdrav" />
	</fieldset>
</form>
