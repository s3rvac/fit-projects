{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	{if $ACTION eq 'view_branches'}
		{if $BRANCHES_DATA}
			<form action="{$U_BRANCHES_DELETE}" method="post">
				<table width="100%" border="0">
					<tr class="table-header">
						<td>
							<a href="{$U_ORDER_BY_NAME_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_BRANCH_NAME}
							<a href="{$U_ORDER_BY_NAME_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td class="view-small-column">{$L_DETAILS_BRANCH}</td>
						<td class="view-small-column">{$L_EDIT_BRANCH}</td>
						<td class="view-small-column">{$L_DELETE_BRANCH}</td>
					</tr>
					{foreach item=branch_data from=$BRANCHES_DATA}
						<tr>
							<td>{$branch_data.name}</td>
							<td class="align-center"><a href="{$branch_data.details_branch_url}">{$L_DETAILS}</a></td>
							<td class="align-center"><a href="{$branch_data.edit_branch_url}">{$L_EDIT}</a></td>
							<td class="align-center"><input class="checkbox" type="checkbox" name="delete_branch[]" value="{$branch_data.branch_id}" /></td>
						</tr>
					{/foreach}
				</table>
				<div class="align-right">
					<input class="submit" type="submit" name="delete_branches" value="{$L_DELETE_SELECTED_BRANCHES}" />
				</div>
			</form>
			{include file='module_pagination.tpl'}
		{else}
			<div class="align-center">
				{$L_NO_BRANCHES_AVAILABLE}
			</div>
		{/if}
	{else}
		<form action="{$U_ACTION_BRANCH}" method="post">
			<table width="100%" border="0">
				<tr>
					<td class="table-label">{$L_BRANCH_NAME}:{if $ACTION neq 'details_branch'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_branch'}<input type="text" name="name" value="{$BRANCH_DATA.name}" />{else}{$BRANCH_DATA.name}{/if}</td>
				</tr>
			</table>
			<div class="action-buttons">
				{if $ACTION neq 'details_branch'}
					<input class="submit" type="submit" name="{$ACTION}" value="{$L_ACTION_BRANCH}" />
				{/if}
					<input class="submit" type="button" onclick="location.replace('{$U_BACK}');" value="{$L_BACK}" />
			</div>
			{if $ACTION neq 'details_branch'}
				<div class="align-left">
					<span class="mandatory-field">*</span> - {$L_MANDATORY_FIELDS}
				</div>
			{/if}
		</form>
	{/if}
{include file='module_footer.tpl'}
