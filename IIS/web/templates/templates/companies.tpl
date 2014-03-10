{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	{if $ACTION eq 'view_companies'}
		{if $COMPANIES_DATA}
			<form action="{$U_COMPANIES_DELETE}" method="post">
				<table width="100%" border="0">
					<tr class="table-header">
						<td>
							<a href="{$U_ORDER_BY_NAME_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_COMPANY_NAME}
							<a href="{$U_ORDER_BY_NAME_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_ADDRESS_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_ADDRESS}
							<a href="{$U_ORDER_BY_ADDRESS_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_IC_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_IC}
							<a href="{$U_ORDER_BY_IC_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td class="view-small-column">{$L_DETAILS_COMPANY}</td>
						<td class="view-small-column">{$L_EDIT_COMPANY}</td>
						<td class="view-small-column">{$L_DELETE_COMPANY}</td>
					</tr>
					{foreach item=company_data from=$COMPANIES_DATA}
						<tr>
							<td>{$company_data.name}</td>
							<td>{$company_data.address}</td>
							<td>{$company_data.IC}</td>
							<td class="align-center"><a href="{$company_data.details_company_url}">{$L_DETAILS}</a></td>
							<td class="align-center"><a href="{$company_data.edit_company_url}">{$L_EDIT}</a></td>
							<td class="align-center"><input class="checkbox" type="checkbox" name="delete_company[]" value="{$company_data.company_id}" /></td>
						</tr>
					{/foreach}
				</table>
				<div class="align-right">
					<input class="submit" type="submit" name="delete_companies" value="{$L_DELETE_SELECTED_COMPANIES}" />
				</div>
			</form>
			{include file='module_pagination.tpl'}
		{else}
			<div class="align-center">
				{$L_NO_COMPANIES_AVAILABLE}
			</div>
		{/if}
	{else}
		<form action="{$U_ACTION_COMPANY}" method="post">
			<table width="100%" border="0">
				<tr>
					<td class="table-label">{$L_COMPANY_NAME}:{if $ACTION neq 'details_company'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_company'}<input type="text" name="name" value="{$COMPANY_DATA.name}" />{else}{$COMPANY_DATA.name}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_ADDRESS}:{if $ACTION neq 'details_company'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_company'}<input type="text" name="address" value="{$COMPANY_DATA.address}" />{else}{$COMPANY_DATA.address}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_EMAIL}:{if $ACTION neq 'details_company'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_company'}<input type="text" name="email" value="{$COMPANY_DATA.email}" />{else}{$COMPANY_DATA.email_url|smarty:nodefaults}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_WWW}:</td>
					<td class="table-input">{if $ACTION neq 'details_company'}<input type="text" name="www" value="{$COMPANY_DATA.www}" />{else}{if $COMPANY_DATA.www}<a href="{$COMPANY_DATA.www}">{$COMPANY_DATA.www}</a>{/if}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_TELEPHONE}:</td>
					<td class="table-input">{if $ACTION neq 'details_company'}<input type="text" name="telephone" value="{$COMPANY_DATA.telephone}" />{else}{$COMPANY_DATA.telephone}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_IC}:{if $ACTION neq 'details_company'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_company'}<input type="text" name="IC" value="{$COMPANY_DATA.IC}" />{else}{$COMPANY_DATA.IC}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_DIC}:</td>
					<td class="table-input">{if $ACTION neq 'details_company'}<input type="text" name="DIC" value="{$COMPANY_DATA.DIC}" />{else}{$COMPANY_DATA.DIC}{/if}</td>
				</tr>
				{if $ACTION eq 'details_company'}
					<tr>
						<td class="table-label">{$L_REPRESENTED_BY}:</td>
						<td class="table-input">
							{if $COMPANY_REPRESENTATIVES}
								{foreach name=company_representatives item=user_data from=$COMPANY_REPRESENTATIVES}
									<a href="{$user_data.details_user_url}">{$user_data.name}</a>{if !$smarty.foreach.company_representatives.last},{/if}
								{/foreach}
							{else}
								-
							{/if}
						</td>
					</tr>
					{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
					<tr>
						<td class="table-label">{$L_CONTRACTS}:</td>
						<td class="table-input">
							{if $COMPANY_CONTRACTS}
								{foreach name=company_contracts item=contract_data from=$COMPANY_CONTRACTS}
									<a href="{$contract_data.details_contract_url}">{$contract_data.contract_id}</a>{if !$smarty.foreach.company_contracts.last},{/if}
								{/foreach}
							{else}
								-
							{/if}
						</td>
					</tr>
					{/if}
				{/if}
			</table>
			<div class="action-buttons">
				{if $ACTION neq 'details_company'}
					<input class="submit" type="submit" name="{$ACTION}" value="{$L_ACTION_COMPANY}" />
				{/if}
					<input class="submit" type="button" onclick="location.replace('{$U_BACK}')" value="{$L_BACK}" />
			</div>
			{if $ACTION neq 'details_company'}
				<div class="align-left">
					<span class="mandatory-field">*</span> - {$L_MANDATORY_FIELDS}
				</div>
			{/if}
		</form>
	{/if}
{include file='module_footer.tpl'}
