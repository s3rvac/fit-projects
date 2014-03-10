{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

<div id="column2">
	<table width="100%" border="0">
		<tr>
			<td>
				<h2>{$PAGE_HEADER}</h2>
				{if $MODULE_ACTIONS and ($USER_GROUP eq $EXHIBITIONGROUND_MEMBER)}
					<div class="page-description">
						{$PAGE_DESCRIPTION}
					</div>
				{else}
					{$PAGE_DESCRIPTION}
				{/if}
			</td>
			{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
			<td class="align-right">
				{if $MODULE_ACTIONS}
					<div class="module-actions">
						<h3>{$L_MODULE_ACTIONS}</h3>
						<br />
						{foreach item=action from=$MODULE_ACTIONS}
							<a href="{$action.url}">{$action.name}</a><br />
						{/foreach}
					</div>
				{/if}
			</td>
			{/if}
		</tr>
	</table>

	<div class="message-centered">
		{if $ERROR_MESSAGE}
			<span class="error-message">{$ERROR_MESSAGE}</span>
		{elseif $SUCCESS_MESSAGE}
			<span class="success-message">{$SUCCESS_MESSAGE}</span>
		{/if}
	</div>
