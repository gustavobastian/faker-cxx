#pragma once

#include <array>
#include <string_view>

namespace faker::database
{
const auto collations = std::to_array<std::string_view>({
    "ascii_bin",
    "ascii_general_ci",
    "cp1250_bin",
    "cp1250_general_ci",
    "utf8_bin",
    "utf8_general_ci",
    "utf8_unicode_ci",
});

const auto columnNames = std::to_array<std::string_view>({
    "avatar",
    "category",
    "comment",
    "created_at",
    "email",
    "first_name",
    "group",
    "id",
    "last_name",
    "name",
    "password",
    "phone",
    "status",
    "title",
    "token",
    "updated_at",
    "username",
    "address",
    "city",
    "country",
    "zip_code",
    "order_id",
    "product_id",
    "product_name",
    "quantity",
    "price",
    "total",
    "payment_method",
    "payment_status",
    "shipping_address",
    "billing_address",
    "date_of_birth",
    "gender",
    "profile_picture",
    "last_login",
    "ip_address",
    "subscription_status",
    "role",
    "membership_type",
    "order_status",
    "coupon_code",
    "discount",
    "expiry_date",
    "shipping_method",
    "tracking_number",
    "review_id",
    "rating",
    "order_date",
    "delivery_date",
    "shipping_date",
    "invoice_number",
    "transaction_id",
    "security_question",
    "security_answer",
    "preferred_language",
    "is_active",
    "is_verified",
    "is_suspended",
    "referral_code",
    "affiliate_id",
    "purchase_date",
    "delivery_address",
    "subscription_plan",
    "payment_gateway",
    "user_ip",
    "profile_visibility",
    "is_deleted",
    "updated_by",
    "created_by",
    "feedback",
    "notes",
});

const auto columnTypes = std::to_array<std::string_view>({
    "bigint", "binary", "bit",      "blob",     "boolean", "date",      "datetime", "decimal",
    "double", "enum",   "float",    "geometry", "int",     "mediumint", "point",    "real",
    "serial", "set",    "smallint", "text",     "time",    "timestamp", "tinyint",  "varchar",
});

const auto engines = std::to_array<std::string_view>({
    "ARCHIVE",
    "BLACKHOLE",
    "CSV",
    "InnoDB",
    "MEMORY",
    "MyISAM",
});

}
