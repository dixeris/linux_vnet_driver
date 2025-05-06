#include <linux/module.h>
#include <linux/netdevice.h>


//First, create the net_device struct by calling alloc_netdev 
//Second, define the operation of the specific device by net_device_ops
//each field in the net_device_ops abstracts how the kernel is interacted with
//specific network devices. we must define operation of each device one by one.
static struct net_device *vnet_dev;
static const struct net_device_ops vnet_netdev_ops;

//init function
static int vnet_start_init(struct net_device *dev) 
{
	pr_info("Initalizing virtual network device\n");
	return 0;
}

//open function 
static int vnet_open(struct net_device *dev)
{

	pr_info("Bringing up the virtual network device\n");
	return 0;
}

// Transmit function
static netdev_tx_t vnet_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    pr_info("vnet: Transmitting packet\n");
    // Free the packet (since there's no actual transmission)
    dev_kfree_skb(skb);
    return NETDEV_TX_OK;
}

// Device setup function
static void vnet_setup(struct net_device *dev)
{
    dev->netdev_ops = &vnet_netdev_ops;
    dev->flags |= IFF_NOARP;
    dev->features |= NETIF_F_HW_CSUM;
}

// Device operations
static const struct net_device_ops vnet_netdev_ops = {
	.ndo_init = vnet_start_init,
    .ndo_start_xmit = vnet_start_xmit,
    .ndo_open       = vnet_open, // Optional
    .ndo_stop       = NULL, // Optional
};

static int __init vnet_init(void)
{
    int result;

    // Allocate the device
    vnet_dev = alloc_netdev(0, "vnet%d", NET_NAME_UNKNOWN, vnet_setup);
    if (!vnet_dev)
        return -ENOMEM;

    // Register the device
    result = register_netdev(vnet_dev);
    if (result < 0) {
        pr_err("vnet: Failed to register device\n");
        free_netdev(vnet_dev);
        return result;
    }

    pr_info("vnet: Virtual device registered\n");
    return 0;
}

static void __exit vnet_exit(void)
{
    unregister_netdev(vnet_dev);
    free_netdev(vnet_dev);
    pr_info("vnet: Virtual device unregistered\n");
}

module_init(vnet_init);
module_exit(vnet_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple Virtual Network Device");

