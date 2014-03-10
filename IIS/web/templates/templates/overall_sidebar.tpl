{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

<div id="column1">
	{foreach key=item_menu_name item=sidebar_item from=$SIDEBAR}
		<div class="sidebar-items">
			<h2>{$item_menu_name}</h2>
			{foreach item=item from=$sidebar_item}
				{if $item}
					{if $item.module_name eq $MODULE_NAME}
						<strong>&#187;</strong>&nbsp;<a href="{$item.url}"><strong>{$item.name}</strong></a>
					{else}
						&#187;&nbsp;<a href="{$item.url}">{$item.name}</a>
					{/if}
				{/if}
				<br />
			{/foreach}
		</div>
	{/foreach}
</div>
