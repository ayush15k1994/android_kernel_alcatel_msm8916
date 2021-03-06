/*
 * EMEA ==> AAC BOX ==> GPIO86=H;
 * LATAM ==> LC BOX ==> GPIO86=L;
 * */
static ssize_t tfa9897_spkbox_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    if (g_tfa9897_sys->spk_box_id)
    {
        return sprintf(buf, "%s\n", "AAC_IDOL347");
    }
    else
    {
        return sprintf(buf, "%s\n", "LC_IDOL347");
    }
}
static ssize_t tfa9897_spkbox_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	b_codec_dbg("%s isn't support\n", __func__);
	return size;
}
static DEVICE_ATTR(spk_box_type, S_IRUGO | S_IWUGO, tfa9897_spkbox_show, tfa9897_spkbox_store);

/*
 * reset
 * */
static ssize_t tfa9897_reset_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return scnprintf(buf, PAGE_SIZE, "%u",  g_tfa9897_sys->reset_pin_state);
}

static ssize_t tfa9897_reset_store(struct device *dev,
        struct device_attribute *attr,
        const char *buf, size_t count)
{
    int err = 0;
    unsigned long value;

    if (count != 1) {
        b_codec_dbg("%s %d cnt=%d, buf=%d\n", __func__, __LINE__, (int)count, (int)sizeof(buf));
        return -EINVAL;
    }

    err = kstrtoul(buf, 10, &value);
    if (err != 0) {
        b_codec_dbg("%s %d err=%d\n", __func__, __LINE__, err);
        return err;
    }

    switch (value)
    {
        case 0:
            tfa9897_reset(g_tfa9897_sys, 0);
            break;

        case 1:
            tfa9897_reset(g_tfa9897_sys, 1);
            break;

        default:
            err = -EINVAL;
            b_codec_dbg("%s %d unknow value err=%d\n", __func__, __LINE__, (int)value);
            break;
    }

    return count;
}
static DEVICE_ATTR(tfa9897_reset, 0666, tfa9897_reset_show, tfa9897_reset_store);


/*
 * switch
 * */
static ssize_t tfa9897_switch1_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return scnprintf(buf, PAGE_SIZE, "%u",  g_tfa9897_sys->switch1_pin_state);
}

static ssize_t tfa9897_switch1_store(struct device *dev,
        struct device_attribute *attr,
        const char *buf, size_t count)
{
    int err = 0;
    unsigned long value;

    if (count != 1) {
        b_codec_dbg("%s %d cnt=%d\n", __func__, __LINE__, (int)count);
        return -EINVAL;
    }

    err = kstrtoul(buf, 10, &value);
    if (err != 0) {
        b_codec_dbg("%s %d err=%d\n", __func__, __LINE__, err);
        return err;
    }

    switch (value)
    {
        case 0:
            tfa9897_switch(g_tfa9897_sys, SPK_REV_TOP_SWITCH, 0);
            break;

        case 1:
            tfa9897_switch(g_tfa9897_sys, SPK_REV_TOP_SWITCH, 1);
            break;

        default:
            err = -EINVAL;
            b_codec_dbg("%s %d unknow value err=%d\n", __func__, __LINE__, (int)value);
            break;
    }

    return count;
}
static DEVICE_ATTR(tfa9897_switch1, 0666, tfa9897_switch1_show, tfa9897_switch1_store);


static ssize_t tfa9897_switch2_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return scnprintf(buf, PAGE_SIZE, "%u",  g_tfa9897_sys->switch2_pin_state);
}

static ssize_t tfa9897_switch2_store(struct device *dev,
        struct device_attribute *attr,
        const char *buf, size_t count)
{
    int err = 0;
    unsigned long value;

    if (count != 1) {
        b_codec_dbg("%s %d cnt=%d\n", __func__, __LINE__, (int)count);
        return -EINVAL;
    }

    err = kstrtoul(buf, 10, &value);
    if (err != 0) {
        b_codec_dbg("%s %d err=%d\n", __func__, __LINE__, err);
        return err;
    }

    switch (value)
    {
        case 0:
            tfa9897_switch(g_tfa9897_sys, SPK_REV_BTM_SWITCH, 0);
            break;

        case 1:
            tfa9897_switch(g_tfa9897_sys, SPK_REV_BTM_SWITCH, 1);
            break;

        default:
            err = -EINVAL;
            b_codec_dbg("%s %d unknow value err=%d\n", __func__, __LINE__, (int)value);
            break;
    }

    return count;
}
static DEVICE_ATTR(tfa9897_switch2, 0666, tfa9897_switch2_show, tfa9897_switch2_store);


/*
 * enable Q4 sclk
 * */
atomic_t sclk_en;
extern int quat_mi2s_sclk_enable(int enable);

static ssize_t tfa9897_sclk_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int value = atomic_read(&sclk_en);
	return sprintf(buf, "%d\n", value);
}

static ssize_t tfa9897_sclk_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	int value;
	sscanf(buf, "%d", &value);
	atomic_set(&sclk_en, value);
	quat_mi2s_sclk_enable(value);
	printk("[Liu]%s: sclk_en=%d\n", __func__, value);
	return size;
}
static DEVICE_ATTR(sclk, S_IRUGO | S_IWUGO, tfa9897_sclk_show, tfa9897_sclk_store);


/*
 * calibrate
 * */
atomic_t calibrate;
static ssize_t tfa9897_calibrate_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int value = atomic_read(&calibrate);
	return sprintf(buf, "%d\n", value);
}

static ssize_t tfa9897_calibrate_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	int value;
	sscanf(buf, "%d", &value);
	atomic_set(&calibrate, value);

	if(value) 
    {
		tfa9897_reset(g_tfa9897_sys, 0);
	}
	else
    {
		tfa9897_reset(g_tfa9897_sys, 1);
	}

	printk("[Liu]%s: calibrate=%d\n", __func__, value);

	return size;
}
static DEVICE_ATTR(calibrate, S_IRUGO | S_IWUGO, tfa9897_calibrate_show, tfa9897_calibrate_store);

static struct class *nxp_class;
static struct device *nxp_dev;
static void tfa9897_sysfs_init(void)
{
	int rc = 0;
	
	nxp_class = class_create(THIS_MODULE, "tfa9897");
	if (IS_ERR(nxp_class)) {
		pr_err("Failed to create class(nxp_class)!\n");
    }

    b_codec_trace();

	nxp_dev = device_create(nxp_class, NULL, 0, NULL, "control");

    b_codec_trace();
    
	rc = device_create_file(nxp_dev, &dev_attr_spk_box_type);
	if (rc) {
		b_codec_err("device_create_file reset eror %d\n", rc);
	}

    b_codec_trace();

    rc = device_create_file(nxp_dev, &dev_attr_tfa9897_reset);
    if (rc) {
        b_codec_err("device_create_file reset eror %d\n", rc);
    }

    b_codec_trace();
    rc = device_create_file(nxp_dev, &dev_attr_tfa9897_switch1);
    if (rc) {
        b_codec_err("device_create_file switch1 eror %d\n", rc);
    }

    b_codec_trace();
    rc = device_create_file(nxp_dev, &dev_attr_tfa9897_switch2);
    if (rc) {
        b_codec_err("device_create_file switch2 eror %d\n", rc);
    }

    b_codec_trace();
    rc = device_create_file(nxp_dev, &dev_attr_sclk);
	if (rc) {
		b_codec_err("device_create_file sclk error %d\n", rc);
	}

    rc = device_create_file(nxp_dev, &dev_attr_calibrate);
    if (rc) {
        b_codec_err("device_create_file calibrate error %d\n", rc);
    }
}
