{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	{if $ACTION eq 'view_users'}
		{if $USERS_DATA}
			<form action="{$U_USERS_DELETE}" method="post">
				<table width="100%" border="0">
					<tr class="table-header">
						<td>
							<a href="{$U_ORDER_BY_USERNAME_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_USERNAME}
							<a href="{$U_ORDER_BY_USERNAME_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_NAME_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_NAME}
							<a href="{$U_ORDER_BY_NAME_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_GROUP_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_GROUP}
							<a href="{$U_ORDER_BY_GROUP_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_COMPANY_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_COMPANY}
							<a href="{$U_ORDER_BY_COMPANY_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td class="view-small-column">{$L_DETAILS_USER}</td>
						<td class="view-small-column">{$L_EDIT_USER}</td>
						<td class="view-small-column">{$L_DELETE_USER}</td>
					</tr>
					{foreach item=user_data from=$USERS_DATA}
						<tr>
							<td>{$user_data.username}</td>
							<td>{$user_data.name}</td>
							<td>{$user_data.group}</td>
							<td>{$user_data.company}</td>
							<td class="align-center"><a href="{$user_data.details_user_url}">{$L_DETAILS}</a></td>
							<td class="align-center"><a href="{$user_data.edit_user_url}">{$L_EDIT}</a></td>
							<td class="align-center"><input class="checkbox" type="checkbox" name="delete_user[]" value="{$user_data.user_id}" /></td>
						</tr>
					{/foreach}
				</table>
				<div class="align-right">
					<input class="submit" type="submit" name="delete_users" value="{$L_DELETE_SELECTED_USERS}" />
				</div>
			</form>
			{include file='module_pagination.tpl'}
		{else}
			<div class="align-center">
				{$L_NO_USERS_AVAILABLE}
			</div>
		{/if}
	{else}
		{literal}
		<script type="text/javascript">
			function update_form()
			{
				var group_id = document.getElementById('group_id');
				var company = document.getElementById('company');

				if (group_id.selectedIndex == {/literal}{$VISITOR}{literal}) {
					company.style.display='';
				} else {
					company.style.display='none';
				}
			}
		</script>
		{/literal}
		<form action="{$U_ACTION_USER}" method="post">
			<table width="100%" border="0">
				{if ($ACTION neq 'details_user') or ($USER_GROUP eq $EXHIBITIONGROUND_MEMBER)}
				<tr>
					<td class="table-label">{$L_USERNAME}:{if $ACTION neq 'details_user'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if ($ACTION eq 'edit_user') or ($ACTION eq 'add_user')}<input type="text" name="username" value="{$USER_DATA.username}" />{else}{$USER_DATA.username}{/if}</td>
				</tr>
				{/if}
				<tr>
					<td class="table-label">{$L_NAME}:{if $ACTION neq 'details_user'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_user'}<input type="text" name="name" value="{$USER_DATA.name}" />{else}{$USER_DATA.name}{/if}</td>
				</tr>
				{if $ACTION neq 'details_user'}
					<tr>
						<td class="table-label">{$L_PASSWORD}:{if $ACTION neq 'details_user'}<span class="mandatory-field">*</span>{/if}</td>
						<td class="table-input"><input type="password" name="password" value="" /></td>
					</tr>
					<tr>
						<td class="table-label">{$L_PASSWORD_CHECK}:{if $ACTION neq 'details_user'}<span class="mandatory-field">*</span>{/if}</td>
						<td class="table-input"><input type="password" name="password_check" value="" /></td>
					</tr>
				{/if}
				<tr>
					<td class="table-label">{$L_EMAIL}:{if $ACTION neq 'details_user'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_user'}<input type="text" name="email" value="{$USER_DATA.email}" />{else}{$USER_DATA.email_url|smarty:nodefaults}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_TELEPHONE}:</td>
					<td class="table-input">{if $ACTION neq 'details_user'}<input type="text" name="telephone" value="{$USER_DATA.telephone}" />{else}{$USER_DATA.telephone}{/if}</td>
				</tr>
				{if $ACTION neq 'profile_user'}
					{if ($ACTION eq 'details_user') or $CAN_HAVE_GROUP_CHANGED}
						<tr>
							<td class="table-label">{$L_GROUP}:{if $ACTION neq 'details_user'}<span class="mandatory-field">*</span>{/if}</td>
							<td class="table-input">
								{if $ACTION neq 'details_user'}
									<select id="group_id" name="group_id" onchange="update_form();">
										{foreach item=group from=$GROUPS}
											<option value="{$group.group_id}" {if $USER_DATA.group_id eq $group.group_id} selected="selected"{/if}>{$group.name}</option>
										{/foreach}
									</select>
								{else}
									{foreach item=group from=$GROUPS}
										{if $USER_DATA.group_id eq $group.group_id}{$group.name}{/if}
									{/foreach}
								{/if}
							</td>
						</tr>
						{if !(($ACTION eq 'details_user') and ($USER_DATA.group_id neq $COMPANY_MEMBER))}
							<tr id="company">
								<td class="table-label">{$L_COMPANY}:{if $ACTION neq 'details_user'}<span class="mandatory-field">*</span>{/if}</td>
								<td class="table-input">
									{if $ACTION neq 'details_user'}
										<select name="company_id">
											{foreach item=company from=$COMPANIES}
												<option value="{$company.company_id}" {if $USER_DATA.company_id eq $company.company_id} selected="selected"{/if}>{$company.name}</option>
											{/foreach}
										</select>
									{else}
										{foreach item=company from=$COMPANIES}
											{if $USER_DATA.company_id eq $company.company_id}<a href="{$USER_DATA.details_company_url}">{$company.name}</a>{/if}
										{/foreach}
									{/if}
								</td>
							</tr>
						{/if}
					{else}
						<tr style="display: none;">
							<td colspan="2">
								<input type="hidden" name="group_id" value="{$USER_DATA.group_id}" />
								<input type="hidden" name="company_id" value="{$USER_DATA.company_id}" />
							</td>
						</tr>
					{/if}
				{/if}
			</table>
			<div class="action-buttons">
				{if $ACTION neq 'details_user'}
					<input class="submit" type="submit" name="{$ACTION}" value="{$L_ACTION_USER}" />
				{/if}
					<input class="submit" type="button" onclick="location.replace('{$U_BACK}');" value="{$L_BACK}" />
			</div>
			{if $ACTION neq 'details_user'}
				<div class="align-left">
					<span class="mandatory-field">*</span> - {$L_MANDATORY_FIELDS}
				</div>
			{/if}
		</form>
		{literal}
		<script type="text/javascript">
			update_form();
		</script>
		{/literal}
	{/if}
{include file='module_footer.tpl'}
