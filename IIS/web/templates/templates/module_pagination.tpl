{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

<div id="pagination">
	{if $PAGINATION}
		{$L_GOTO_PAGE}:
		{foreach name=pagination item=url key=page_number from=$PAGINATION}
			<a href="{$url}">{if $CURRENT_PAGE_NUMBER eq $page_number}<strong>{/if}{$page_number}{if $CURRENT_PAGE_NUMBER eq $page_number}</strong>{/if}</a>{if !$smarty.foreach.pagination.last},{/if}
		{/foreach}
	{/if}
</div>
