{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	{if $ACTION eq 'view_exhibitions'}
		{if $EXHIBITIONS_DATA}
			<form action="{$U_EXHIBITIONS_DELETE}" method="post">
				<table width="100%" border="0">
					<tr class="table-header">
						<td>
							<a href="{$U_ORDER_BY_NAME_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_EXHIBITION_NAME}
							<a href="{$U_ORDER_BY_NAME_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_START_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_EXHIBITION_START}
							<a href="{$U_ORDER_BY_START_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_END_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_EXHIBITION_END}
							<a href="{$U_ORDER_BY_END_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td class="view-small-column">{$L_DETAILS_EXHIBITION}</td>
						<td class="view-small-column">{$L_EDIT_EXHIBITION}</td>
						<td class="view-small-column">{$L_DELETE_EXHIBITION}</td>
					</tr>
					{foreach item=exhibition_data from=$EXHIBITIONS_DATA}
						<tr>
							<td>{$exhibition_data.name}</td>
							<td>{$exhibition_data.start}</td>
							<td>{$exhibition_data.end}</td>
							<td class="align-center"><a href="{$exhibition_data.details_exhibition_url}">{$L_DETAILS}</a></td>
							<td class="align-center"><a href="{$exhibition_data.edit_exhibition_url}">{$L_EDIT}</a></td>
							<td class="align-center"><input class="checkbox" type="checkbox" name="delete_exhibition[]" value="{$exhibition_data.exhibition_id}" /></td>
						</tr>
					{/foreach}
				</table>
				<div class="align-right">
					<input class="submit" type="submit" name="delete_exhibitions" value="{$L_DELETE_SELECTED_EXHIBITIONS}" />
				</div>
			</form>
			{include file='module_pagination.tpl'}
		{else}
			<div class="align-center">
				{$L_NO_EXHIBITIONS_AVAILABLE}
			</div>
		{/if}
	{else}
		<form action="{$U_ACTION_EXHIBITION}" method="post">
			<table width="100%" border="0">
				<tr>
					<td class="table-label">{$L_EXHIBITION_NAME}:{if $ACTION neq 'details_exhibition'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_exhibition'}<input type="text" name="name" value="{$EXHIBITION_DATA.name}" />{else}{$EXHIBITION_DATA.name}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_EXHIBITION_START}:{if $ACTION neq 'details_exhibition'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_exhibition'}<input type="text" name="start" value="{$EXHIBITION_DATA.start}" />{else}{$EXHIBITION_DATA.start}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_EXHIBITION_END}:{if $ACTION neq 'details_exhibition'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_exhibition'}<input type="text" name="end" value="{$EXHIBITION_DATA.end}" />{else}{$EXHIBITION_DATA.end}{/if}</td>
				</tr>
			</table>
			<div class="action-buttons">
				{if $ACTION neq 'details_exhibition'}
					<input class="submit" type="submit" name="{$ACTION}" value="{$L_ACTION_EXHIBITION}" />
				{/if}
					<input class="submit" type="button" onclick="location.replace('{$U_BACK}');" value="{$L_BACK}" />
			</div>
			{if $ACTION neq 'details_exhibition'}
				<div class="align-left">
					<span class="mandatory-field">*</span> - {$L_MANDATORY_FIELDS}
				</div>
			{/if}
		</form>
	{/if}
{include file='module_footer.tpl'}
