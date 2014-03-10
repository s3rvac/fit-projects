{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	{if $ACTION eq 'view_contracts'}
		{if $CONTRACTS_DATA}
			<form action="{$U_CONTRACTS_DELETE}" method="post">
				<table width="100%" border="0">
					<tr class="table-header">
						<td>
							<a href="{$U_ORDER_BY_CONTRACT_NUMBER_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_CONTRACT_N}
							<a href="{$U_ORDER_BY_CONTRACT_NUMBER_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_COMPANY_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_COMPANY}
							<a href="{$U_ORDER_BY_COMPANY_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_CONCLUSION_DATE_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_CONCLUSION_DATE}
							<a href="{$U_ORDER_BY_CONCLUSION_DATE_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td class="view-small-column">{$L_DETAILS_CONTRACT}</td>
						{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
							<td class="view-small-column">{$L_EDIT_CONTRACT}</td>
							<td class="view-small-column">{$L_DELETE_CONTRACT}</td>
						{/if}
					</tr>
					{foreach item=contract_data from=$CONTRACTS_DATA}
						<tr>
							<td>{$contract_data.contract_id}</td>
							<td>{$contract_data.company}</td>
							<td>{$contract_data.conclusion_date}</td>
							<td class="align-center"><a href="{$contract_data.details_contract_url}">{$L_DETAILS}</a></td>
							{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
								<td class="align-center"><a href="{$contract_data.edit_contract_url}">{$L_EDIT}</a></td>
								<td class="align-center"><input class="checkbox" type="checkbox" name="delete_contract[]" value="{$contract_data.contract_id}" /></td>
							{/if}
						</tr>
					{/foreach}
				</table>
				{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
					<div class="align-right">
						<input class="submit" type="submit" name="delete_contracts" value="{$L_DELETE_SELECTED_CONTRACTS}" />
					</div>
				{/if}
			</form>
			{include file='module_pagination.tpl'}
		{else}
			<div class="align-center">
				{$L_NO_CONTRACTS_AVAILABLE}
			</div>
		{/if}
	{else}
		<form action="{$U_ACTION_CONTRACT}" method="post">
			<table width="100%" border="0">
				{if $ACTION eq 'details_contract' or $ACTION eq 'edit_contract'}
					<tr>
						<td class="table-label">{$L_CONTRACT_NUMBER}:</td>
						<td class="table-input">{$CONTRACT_DATA.contract_id}</td>
					</tr>
				{/if}
				<tr>
					<td class="table-label">{$L_COMPANY}:{if $ACTION neq 'details_contract'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">
						{if $ACTION neq 'details_contract'}
							<select name="company_id">
								{foreach item=company from=$COMPANIES}
									<option value="{$company.company_id}" {if $CONTRACT_DATA.company_id eq $company.company_id} selected="selected"{/if}>{$company.name}</option>
								{/foreach}
							</select>
						{else}
							{foreach item=company from=$COMPANIES}
								{if $CONTRACT_DATA.company_id eq $company.company_id}<a href="{$CONTRACT_DATA.details_company_url}">{$company.name}</a>{/if}
							{/foreach}
						{/if}
					</td>
				</tr>
				<tr>
					<td class="table-label">{$L_CONCLUDED_BY}:{if $ACTION neq 'details_contract'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">
						{if $ACTION neq 'details_contract'}
							<select name="concluded_by">
								{foreach item=user from=$USERS}
									<option value="{$user.user_id}" {if $CONTRACT_DATA.concluded_by eq $user.user_id} selected="selected"{/if}>{$user.company} - {$user.name}</option>
								{/foreach}
							</select>
						{else}
							{foreach item=user from=$USERS}
        						{if $CONTRACT_DATA.concluded_by eq $user.user_id}<a href="{$CONTRACT_DATA.details_user_url}">{$user.name}</a>{/if}
							{/foreach}
						{/if}
					</td>
				</tr>
				<tr>
					<td class="table-label">{$L_CONCLUSION_DATE}:{if $ACTION neq 'details_contract'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_contract'}<input type="text" name="conclusion_date" value="{$CONTRACT_DATA.conclusion_date}" />{else}{$CONTRACT_DATA.conclusion_date}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_VALIDITY_START}:{if $ACTION neq 'details_contract'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_contract'}<input type="text" name="validity_start" value="{$CONTRACT_DATA.validity_start}" />{else}{$CONTRACT_DATA.validity_start}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_VALIDITY_END}:{if $ACTION neq 'details_contract'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_contract'}<input type="text" name="validity_end" value="{$CONTRACT_DATA.validity_end}" />{else}{$CONTRACT_DATA.validity_end}{/if}</td>
				</tr>
				{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
					<tr>
						<td class="table-label">{$L_MATURITY_DATE}:{if $ACTION neq 'details_contract'}<span class="mandatory-field">*</span>{/if}</td>
						<td class="table-input">{if $ACTION neq 'details_contract'}<input type="text" name="maturity_date" value="{$CONTRACT_DATA.maturity_date}" />{else}{$CONTRACT_DATA.maturity_date}{/if}</td>
					</tr>
					{if $ACTION eq 'details_contract'}
						<tr>
							<td class="table-label">{$L_EXPOSITIONS}:</td>
							<td class="table-input">
								{if $CONTRACT_EXPOSITIONS}
									{foreach name=contract_expositions item=exposition_data from=$CONTRACT_EXPOSITIONS}
										<a href="{$exposition_data.details_exposition_url}">{$smarty.foreach.contract_expositions.iteration}</a>{if !$smarty.foreach.contract_expositions.last},{/if}
									{/foreach}
								{else}
									-
								{/if}
							</td>
						</tr>
					{/if}
					{if $ACTION neq 'add_contract'}
						<tr>
							<td class="table-label">{$L_TOTAL_PRICE}:</td>
							<td class="table-input">{$CONTRACT_DATA.price}</td>
						</tr>
						<tr>
							<td class="table-label">{$L_SETTLED}:</td>
							<td class="table-input">{if $ACTION neq 'details_contract'}<input type="text" name="settled" value="{$CONTRACT_DATA.settled}" />{else}{$CONTRACT_DATA.settled}{/if}</td>
						</tr>
					{/if}
				{/if}
			</table>
			<div class="action-buttons">
				{if $ACTION neq 'details_contract'}
					<input class="submit" type="submit" name="{$ACTION}" value="{$L_ACTION_CONTRACT}" />
				{/if}
					<input class="submit" type="button" onclick="location.replace('{$U_BACK}');" value="{$L_BACK}" />
			</div>
			{if $ACTION neq 'details_contract'}
				<div class="align-left">
					<span class="mandatory-field">*</span> - {$L_MANDATORY_FIELDS}
				</div>
			{/if}
		</form>
	{/if}
{include file='module_footer.tpl'}
