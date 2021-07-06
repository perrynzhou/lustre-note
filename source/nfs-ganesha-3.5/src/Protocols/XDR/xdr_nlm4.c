/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "config.h"
#include "gsh_rpc.h"
#include "nlm4.h"
#include "nfs_fh.h"

bool xdr_nlm4_stats(XDR * xdrs, nlm4_stats * objp)
{
	if (!xdr_enum(xdrs, (enum_t *) objp))
		return false;
	return true;
}

bool xdr_nlm4_stat(XDR * xdrs, nlm4_stat * objp)
{
	if (!xdr_nlm4_stats(xdrs, &objp->stat))
		return false;
	return true;
}

bool xdr_nlm4_res(XDR * xdrs, nlm4_res * objp)
{
	if (!xdr_netobj(xdrs, &objp->cookie))
		return false;
	if (!xdr_nlm4_stat(xdrs, &objp->stat))
		return false;
	return true;
}

bool xdr_nlm4_holder(XDR * xdrs, nlm4_holder * objp)
{
	if (!xdr_bool(xdrs, &objp->exclusive))
		return false;
	if (!xdr_int32_t(xdrs, &objp->svid))
		return false;
	if (!xdr_netobj(xdrs, &objp->oh))
		return false;
	if (!xdr_uint64_t(xdrs, &objp->l_offset))
		return false;
	if (!xdr_uint64_t(xdrs, &objp->l_len))
		return false;
	return true;
}

bool xdr_nlm4_testrply(XDR * xdrs, nlm4_testrply * objp)
{
	if (!xdr_nlm4_stats(xdrs, &objp->stat))
		return false;
	switch (objp->stat) {
	case NLM4_DENIED:
		if (!xdr_nlm4_holder(xdrs, &objp->nlm4_testrply_u.holder))
			return false;
		break;
	default:
		break;
	}
	return true;
}

bool xdr_nlm4_testres(XDR * xdrs, nlm4_testres * objp)
{
	if (!xdr_netobj(xdrs, &objp->cookie))
		return false;
	if (!xdr_nlm4_testrply(xdrs, &objp->test_stat))
		return false;
	return true;
}

bool xdr_nlm4_lock(XDR * xdrs, nlm4_lock * objp)
{
	if (!xdr_string(xdrs, &objp->caller_name, LM_MAXSTRLEN))
		return false;
	if (!xdr_netobj(xdrs, &objp->fh))
		return false;
	if (!xdr_netobj(xdrs, &objp->oh))
		return false;
	if (!xdr_int32_t(xdrs, &objp->svid))
		return false;
	if (!xdr_uint64_t(xdrs, &objp->l_offset))
		return false;
	if (!xdr_uint64_t(xdrs, &objp->l_len))
		return false;
	return true;
}

bool xdr_nlm4_lockargs(XDR * xdrs, nlm4_lockargs * objp)
{
	if (!xdr_netobj(xdrs, &objp->cookie))
		return false;
	if (!xdr_bool(xdrs, &objp->block))
		return false;
	if (!xdr_bool(xdrs, &objp->exclusive))
		return false;
	if (!xdr_nlm4_lock(xdrs, &objp->alock))
		return false;
	if (!xdr_bool(xdrs, &objp->reclaim))
		return false;
	if (!xdr_int32_t(xdrs, &objp->state))
		return false;
	return true;
}

bool xdr_nlm4_cancargs(XDR * xdrs, nlm4_cancargs * objp)
{
	if (!xdr_netobj(xdrs, &objp->cookie))
		return false;
	if (!xdr_bool(xdrs, &objp->block))
		return false;
	if (!xdr_bool(xdrs, &objp->exclusive))
		return false;
	if (!xdr_nlm4_lock(xdrs, &objp->alock))
		return false;
	return true;
}

bool xdr_nlm4_testargs(XDR * xdrs, nlm4_testargs * objp)
{
	if (!xdr_netobj(xdrs, &objp->cookie))
		return false;
	if (!xdr_bool(xdrs, &objp->exclusive))
		return false;
	if (!xdr_nlm4_lock(xdrs, &objp->alock))
		return false;
	return true;
}

bool xdr_nlm4_unlockargs(XDR * xdrs, nlm4_unlockargs * objp)
{
	if (!xdr_netobj(xdrs, &objp->cookie))
		return false;
	if (!xdr_nlm4_lock(xdrs, &objp->alock))
		return false;
	return true;
}

bool xdr_fsh4_mode(XDR * xdrs, fsh4_mode * objp)
{
	if (!xdr_enum(xdrs, (enum_t *) objp))
		return false;
	return true;
}

bool xdr_fsh4_access(XDR * xdrs, fsh4_access * objp)
{
	if (!xdr_enum(xdrs, (enum_t *) objp))
		return false;
	return true;
}

bool xdr_nlm4_share(XDR * xdrs, nlm4_share * objp)
{
	if (!xdr_string(xdrs, &objp->caller_name, LM_MAXSTRLEN))
		return false;
	if (!xdr_netobj(xdrs, &objp->fh))
		return false;
	if (!xdr_netobj(xdrs, &objp->oh))
		return false;
	if (!xdr_fsh4_mode(xdrs, &objp->mode))
		return false;
	if (!xdr_fsh4_access(xdrs, &objp->access))
		return false;
	return true;
}

bool xdr_nlm4_shareargs(XDR * xdrs, nlm4_shareargs * objp)
{
	if (!xdr_netobj(xdrs, &objp->cookie))
		return false;
	if (!xdr_nlm4_share(xdrs, &objp->share))
		return false;
	if (!xdr_bool(xdrs, &objp->reclaim))
		return false;
	return true;
}

bool xdr_nlm4_shareres(XDR * xdrs, nlm4_shareres * objp)
{
	if (!xdr_netobj(xdrs, &objp->cookie))
		return false;
	if (!xdr_nlm4_stats(xdrs, &objp->stat))
		return false;
	if (!xdr_int32_t(xdrs, &objp->sequence))
		return false;
	return true;
}

bool xdr_nlm4_free_allargs(XDR * xdrs, nlm4_free_allargs * objp)
{
	if (!xdr_string(xdrs, &objp->name, LM_MAXNAMELEN))
		return false;
	if (!xdr_uint32_t(xdrs, &objp->state))
		return false;
	return true;
}

bool xdr_nlm4_sm_notifyargs(XDR * xdrs, nlm4_sm_notifyargs * objp)
{
	if (!xdr_string(xdrs, &objp->name, SM_MAXSTRLEN))
		return false;
	if (!xdr_int32_t(xdrs, &objp->state))
		return false;
	if (!xdr_opaque(xdrs, objp->priv, SM_PRIV_SZ))
		return false;
	return true;
}
