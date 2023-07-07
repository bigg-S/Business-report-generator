

#include "retrieve_data.h"

/*Holder of a found document*/
bsoncxx::stdx::optional<bsoncxx::document::value> mproduct;

/*Holder of a view of a document*/
bsoncxx::document::view mproduct_view;

/*Holder of an elemnt*/
bsoncxx::document::element mcurrent_amount;

int get_location_data(mongocxx::collection products, SALE sale)
{
    mproduct = products.find_one(document{} << "name" << sale.product_name << finalize);
    mproduct_view = mproduct.value().view();
    mcurrent_amount = mproduct_view["Brands"][sale.brand][sale.quantity][sale.season][sale.age_group][sale.gender][sale.location];

    return mcurrent_amount.get_int32();
}

int get_gender_total(mongocxx::collection products, SALE sale)
{
    mproduct = products.find_one(document{} << "name" << sale.product_name << finalize);
    mproduct_view = mproduct.value().view();
    mcurrent_amount = mproduct_view["Brands"][sale.brand][sale.quantity][sale.season][sale.age_group][sale.gender]["Total"];

    return mcurrent_amount.get_int32();
}


int get_age_group_total(mongocxx::collection products, SALE sale)
{
    mproduct = products.find_one(document{} << "name" << sale.product_name << finalize);
    mproduct_view = mproduct.value().view();
    mcurrent_amount = mproduct_view["Brands"][sale.brand][sale.quantity][sale.season][sale.age_group]["Total"];

    return mcurrent_amount.get_int32();
}

int get_season_total(mongocxx::collection products, SALE sale)
{
    mproduct = products.find_one(document{} << "name" << sale.product_name << finalize);
    mproduct_view = mproduct.value().view();
    mcurrent_amount = mproduct_view["Brands"][sale.brand][sale.quantity][sale.season]["Total"];

    return mcurrent_amount.get_int32();
}

int get_quantity_total(mongocxx::collection products, SALE sale)
{   
    mproduct = products.find_one(document{} << "name" << sale.product_name << finalize);
    mproduct_view = mproduct.value().view();
    mcurrent_amount = mproduct_view["Brands"][sale.brand][sale.quantity]["Total"];

    return mcurrent_amount.get_int32();
}

double get_quantity_unit_cost(mongocxx::collection products, SALE sale, std::string price_type)
{
    mproduct = products.find_one(document{} << "name" << sale.product_name << finalize);
    mproduct_view = mproduct.value().view();
    mcurrent_amount = mproduct_view["Brands"][sale.brand][sale.quantity][price_type];

    if (mcurrent_amount.type() == bsoncxx::type::k_double)
        return mcurrent_amount.get_double();
    else if (mcurrent_amount.type() == bsoncxx::type::k_int64)
        return (double)mcurrent_amount.get_int64();
    else if (mcurrent_amount.type() == bsoncxx::type::k_int32)
        return (double)mcurrent_amount.get_int32();
    else
        return 0.0;
}


int get_brand_total(mongocxx::collection products, SALE sale)
{
    mproduct = products.find_one(document{} << "name" << sale.product_name << finalize);
    mproduct_view = mproduct.value().view();
    mcurrent_amount = mproduct_view["Brands"][sale.brand]["Total"];

    if (mcurrent_amount.raw() == NULL)
        return 0;
    else
     return mcurrent_amount.get_int32();
}


int get_product_total(mongocxx::collection products, SALE sale)
{
    mproduct = products.find_one(document{} << "name" << sale.product_name << finalize);
    mproduct_view = mproduct.value().view();
    mcurrent_amount = mproduct_view["Brands"]["Total"];

    if (mcurrent_amount.raw() == NULL)
        return 0;
    else
        return mcurrent_amount.get_int32();
}



std::string get_all_common_entity_totals(mongocxx::collection products, SALE sale, std::string entity)
{
    std::string details = "";
    int amount = 0;

    mproduct = products.find_one(document{} << "name" << "Components" << finalize);
    mproduct_view = mproduct.value().view();
    bsoncxx::array::view entities = mproduct_view[entity].get_array();

    for (bsoncxx::array::element my_entity : entities)
    {
        if (entity.compare("Locations") == 0)
        {
            sale.location = my_entity.get_string().value.to_string();
            sale.brand = sale.product_name + "_" + sale.brand;
            amount = get_location_data(products, sale);
        }            
        else if (entity.compare("Genders") == 0)
        {
            sale.gender = my_entity.get_string().value.to_string();
            sale.brand = sale.product_name + "_" + sale.brand;
            amount = get_gender_total(products, sale);
        }
            
        else if (entity.compare("Age Groups") == 0)
        {
            sale.age_group = my_entity.get_string().value.to_string();
            sale.brand = sale.product_name + "_" + sale.brand;
            amount = get_age_group_total(products, sale);
        }
            
        else if (entity.compare("Seasons") == 0)
        {
            sale.season = my_entity.get_string().value.to_string();
            sale.brand = sale.product_name + "_" + sale.brand;
            amount = get_season_total(products, sale);
        }
            
        else
            return "";
        
        details += my_entity.get_string().value.to_string() + ":";        

        details += amount + ",";
    }

    return details;
}


std::string get_all_quantites_totals(mongocxx::collection products, SALE sale)
{
    std::string details = "";
    int amount = 0;

    mproduct = products.find_one(document{} << "name" << "Components" << finalize);
    mproduct_view = mproduct.value().view();

    bsoncxx::array::view quantities;
    
    bsoncxx::array::view brands = mproduct_view["Brands"][sale.product_name].get_array();

    for (bsoncxx::array::element my_brand : brands)
    {
        quantities = mproduct_view["Quantities"][my_brand.get_string().value.to_string()].get_array();

        for (bsoncxx::array::element my_quantity : quantities)
        {
            sale.brand    = my_brand.get_string().value.to_string();

            sale.quantity = my_quantity.get_string().value.to_string();
            
            details += my_brand.get_string().value.to_string()  + "_" + my_quantity.get_string().value.to_string() + ":";

            sale.brand = sale.product_name + "_" + sale.brand;
            amount = get_quantity_total(products, sale);

            details += amount + ",";
        }
    }

    return details;
}


std::string get_all_brands_totals(mongocxx::collection products, SALE sale)
{
    std::string details = "";
    int amount = 0;

    mproduct = products.find_one(document{} << "name" << "Components" << finalize);
    mproduct_view = mproduct.value().view();

    bsoncxx::array::view brands = mproduct_view["Brands"][sale.product_name].get_array();

    for (bsoncxx::array::element my_brand : brands)
    {
      
      sale.brand = my_brand.get_string().value.to_string();

      details += my_brand.get_string().value.to_string()  + ":";

      sale.brand = sale.product_name + "_" + sale.brand;
      amount = get_brand_total(products, sale);

      details += amount + ",";
        
    }

    return details;
}

std::string get_all_products_totals(mongocxx::collection products, SALE sale)
{
    std::string details = "";
    int amount = 0;

    mproduct = products.find_one(document{} << "name" << "Components" << finalize);
    mproduct_view = mproduct.value().view();

    bsoncxx::array::view products_array = mproduct_view["Products"].get_array();

    for (bsoncxx::array::element my_product : products_array)
    {

        sale.product_name = my_product.get_string().value.to_string();

        details += my_product.get_string().value.to_string() + ":";

        amount = get_product_total(products, sale);

        details += amount + ",";

    }

    return details;
}


std::vector<std::string> get_products_array(mongocxx::collection products)
{
    std::vector<std::string> details;
    
    mproduct = products.find_one(document{} << "name" << "Components" << finalize);
    mproduct_view = mproduct.value().view();

    bsoncxx::array::view products_array = mproduct_view["Products"].get_array();

    for (bsoncxx::array::element my_product : products_array)
    {
        details.push_back(my_product.get_string().value.to_string());
    }

    return details;
}

std::vector<std::string> get_product_brands_array(mongocxx::collection products, std::string product_name)
{
    std::vector<std::string> details;
    
    mproduct = products.find_one(document{} << "name" << "Components" << finalize);
    mproduct_view = mproduct.value().view();

    bsoncxx::array::view brands = mproduct_view["Brands"][product_name].get_array();

    for (bsoncxx::array::element my_brand : brands)
    {  
        std::string b_name = my_brand.get_string().value.to_string();
        
        b_name = b_name.substr(b_name.find_first_of('_', 0) + 1, b_name.size() - 1);

        details.push_back(b_name);
    }

    return details;
}

std::vector<std::string> get_brand_quantities_array(mongocxx::collection products, std::string brand_name)
{
    std::vector<std::string> details;
    
    mproduct = products.find_one(document{} << "name" << "Components" << finalize);
    mproduct_view = mproduct.value().view();

    bsoncxx::array::view quantities;

    quantities = mproduct_view["Quantities"][brand_name].get_array();

    for (bsoncxx::array::element my_quantity : quantities)
    {        
        details.push_back(my_quantity.get_string().value.to_string());
    }    

    return details;
}

std::vector<int> get_brand_amounts_array(mongocxx::collection products, mongocxx::collection stock, std::string product_name, std::string brand_name)
{
    std::vector<int> details;
    int amount = 0;

    SALE sale;
    sale.product_name = product_name;
    sale.brand = sale.product_name + "_" + brand_name;

    bsoncxx::stdx::optional<bsoncxx::document::value> myproduct = products.find_one(document{} << "name" << "Components" << finalize);
    bsoncxx::document::view  myproduct_view = myproduct.value().view();
    
    
    bsoncxx::array::view mquantities;

    mquantities = myproduct_view["Quantities"][sale.brand].get_array();

    for (bsoncxx::array::element my_quantity : mquantities)
    {
        
        sale.quantity = my_quantity.get_string().value.to_string();

        amount = get_quantity_total(stock, sale);

        details.push_back(amount);
    }

    return details;
}



std::vector<std::string> get_seasons_array(mongocxx::collection products)
{
    std::vector<std::string> details;

    bsoncxx::stdx::optional<bsoncxx::document::value> myproduct = products.find_one(document{} << "name" << "Components" << finalize);
    bsoncxx::document::view  myproduct_view = myproduct.value().view();

    bsoncxx::array::view mseasons;

    mseasons = myproduct_view["Seasons"].get_array();

    for (bsoncxx::array::element my_season : mseasons)
    {
        details.push_back(my_season.get_string().value.to_string());
    }

    return details;
}

std::vector<std::string> get_age_groups_array(mongocxx::collection products)
{
    std::vector<std::string> details;

    bsoncxx::stdx::optional<bsoncxx::document::value> myproduct = products.find_one(document{} << "name" << "Components" << finalize);
    bsoncxx::document::view  myproduct_view = myproduct.value().view();

    bsoncxx::array::view mage_groups;

    mage_groups = myproduct_view["Age Groups"].get_array();

    for (bsoncxx::array::element my_age_group : mage_groups)
    {
        details.push_back (my_age_group.get_string().value.to_string());
    }

    return details;
}

std::vector<std::string> get_genders_array(mongocxx::collection products)
{
    std::vector<std::string> details;

    bsoncxx::stdx::optional<bsoncxx::document::value> myproduct = products.find_one(document{} << "name" << "Components" << finalize);
    bsoncxx::document::view  myproduct_view = myproduct.value().view();

    bsoncxx::array::view mgenders;

    mgenders = myproduct_view["Genders"].get_array();

    for (bsoncxx::array::element my_gender : mgenders)
    {
        details.push_back(my_gender.get_string().value.to_string());
    }

    return details;
}

std::vector<std::string> get_locations_array(mongocxx::collection products)
{
    std::vector<std::string> details;

    bsoncxx::stdx::optional<bsoncxx::document::value> myproduct = products.find_one(document{} << "name" << "Components" << finalize);
    bsoncxx::document::view  myproduct_view = myproduct.value().view();

    bsoncxx::array::view mlocations;

    mlocations = myproduct_view["Locations"].get_array();

    for (bsoncxx::array::element my_location : mlocations)
    {
        details.push_back(my_location.get_string().value.to_string());
    }

    return details;
}


double get_operational_costs_total(mongocxx::collection operational_costs)
{

    bsoncxx::stdx::optional<bsoncxx::document::value> operational_cost = operational_costs.find_one(document{} << "name" << "OperationalCosts" << finalize);
    bsoncxx::document::view operational_cost_view = operational_cost.value().view();
    bsoncxx::document::element mcurrent_amount = operational_cost_view["Costs"]["Total"];

    if (mcurrent_amount.type() == bsoncxx::type::k_double)
        return mcurrent_amount.get_double();
    else if (mcurrent_amount.type() == bsoncxx::type::k_int64)
        return (double)mcurrent_amount.get_int64();
    else if (mcurrent_amount.type() == bsoncxx::type::k_int32)
        return (double)mcurrent_amount.get_int32();
    else
        return 0.0;

}

double get_expected_expenditure(mongocxx::collection operational_costs)
{
    bsoncxx::stdx::optional<bsoncxx::document::value> operational_cost = operational_costs.find_one(document{} << "name" << "OperationalCosts" << finalize);
    bsoncxx::document::view operational_cost_view = operational_cost.value().view();
    bsoncxx::document::element mcurrent_amount = operational_cost_view["Expected Expenditure"];

    if (mcurrent_amount.type() == bsoncxx::type::k_double)
        return mcurrent_amount.get_double();
    else if (mcurrent_amount.type() == bsoncxx::type::k_int64)
        return (double)mcurrent_amount.get_int64();
    else if (mcurrent_amount.type() == bsoncxx::type::k_int32)
        return (double)mcurrent_amount.get_int32();
    else
        return 0.0;
}

double get_operational_costs_data(mongocxx::collection operational_costs, std::string operation_description)
{

    bsoncxx::stdx::optional<bsoncxx::document::value> operational_cost = operational_costs.find_one(document{} << "name" << "OperationalCosts" << finalize);
    bsoncxx::document::view operational_cost_view = operational_cost.value().view();
    bsoncxx::document::element mcurrent_amount = operational_cost_view["Costs"][operation_description];

    if (mcurrent_amount.type() == bsoncxx::type::k_double)
        return mcurrent_amount.get_double();
    else if (mcurrent_amount.type() == bsoncxx::type::k_int64)
        return (double)mcurrent_amount.get_int64();
    else if (mcurrent_amount.type() == bsoncxx::type::k_int32)
        return (double)mcurrent_amount.get_int32();
    else
        return 0.0;
}


std::string get_all_operational_costs_totals(mongocxx::collection operational_costs)
{
    std::string details = "";
    double amount = 0.0;

    bsoncxx::stdx::optional<bsoncxx::document::value> operational_cost = operational_costs.find_one(document{} << "name" << "OperationalCosts" << finalize);
    bsoncxx::document::view operational_cost_view = operational_cost.value().view();

    bsoncxx::array::view costs_array = operational_cost_view["Costs"]["List"].get_array();

    for (bsoncxx::array::element my_cost_description : costs_array)
    {
        details += my_cost_description.get_string().value.to_string() + ":";

        amount = get_operational_costs_data(operational_costs, my_cost_description.get_string().value.to_string());

        details += std::to_string(amount) + ",";

    }

    return details;
}

std::vector<std::string> get_expenses_array(mongocxx::collection operational_costs)
{
    std::vector<std::string> details;

    bsoncxx::stdx::optional<bsoncxx::document::value> operational_cost = operational_costs.find_one(document{} << "name" << "OperationalCosts" << finalize);
    bsoncxx::document::view operational_cost_view = operational_cost.value().view();

    bsoncxx::array::view costs_array = operational_cost_view["Costs"]["List"].get_array();

    for (bsoncxx::array::element my_cost_description : costs_array)
    {
        details.push_back(my_cost_description.get_string().value.to_string());
    }

    return details;
}


//for the report generation

std::vector<CategoryData> get_seasons_data(mongocxx::collection products)
{
    std::vector<CategoryData> seasons_data;    

    bsoncxx::stdx::optional<bsoncxx::document::value> my_components = products.find_one(document{} << "name" << "Components" << finalize);
    bsoncxx::document::view  my_components_view = my_components.value().view();

    bsoncxx::stdx::optional<bsoncxx::document::value> product;
    bsoncxx::document::view product_view;

    bsoncxx::array::view mseasons = my_components_view["Seasons"].get_array();
    bsoncxx::array::view mproducts = my_components_view["Products"].get_array();

    bsoncxx::array::view mbrands;
    bsoncxx::array::view mquantities;

   
    for (bsoncxx::array::element m_season : mseasons)
    {     
        CategoryData c_data;
        c_data.category_name = m_season.get_string().value.to_string();

        for (bsoncxx::array::element m_product : mproducts)
        {
            ProductData p_data;
            p_data.product_name = m_product.get_string().value.to_string();

            product = products.find_one(document{} << "name" << m_product.get_string().value.to_string().c_str() << finalize);
            product_view = product.value().view();

            mbrands = my_components_view["Brands"][m_product.get_string().value.to_string()].get_array();

            for (bsoncxx::array::element m_brand : mbrands)
            {
                BrandData b_data;
                b_data.brand_name = m_brand.get_string().value.to_string();
                size_t name_separator_index = b_data.brand_name.find_first_of('_', 0);
                b_data.brand_name = b_data.brand_name.substr(name_separator_index + 1, b_data.brand_name.size() - name_separator_index);


                mquantities = my_components_view["Quantities"][m_brand.get_string().value.to_string()].get_array();
                
                for (bsoncxx::array::element m_quantity : mquantities)
                {
                    QuantityData q_data;
                    q_data.quantity_name = m_quantity.get_string().value.to_string();
                    q_data.quantity_total = product_view["Brands"][m_brand.get_string().value.to_string()][m_quantity.get_string().value.to_string()][m_season.get_string().value.to_string()]["Total"].get_int32();
                    
                    b_data.brand_total += q_data.quantity_total;//update brand total
                    b_data.b_quantities.push_back(q_data);

                    std::sort(b_data.b_quantities.begin(), b_data.b_quantities.end(),
                        [](QuantityData a, QuantityData b)
                        {
                            return a.quantity_total > b.quantity_total;
                        }
                    );
                    
                }
                p_data.product_total += b_data.brand_total; //update product total
                p_data.p_brands.push_back(b_data);

                std::sort(p_data.p_brands.begin(), p_data.p_brands.end(),
                    [](BrandData a, BrandData b)
                    {
                        return a.brand_total > b.brand_total;
                    }
                );
            }

            c_data.category_total += p_data.product_total;
            c_data.p_products.push_back(p_data);

            std::sort(c_data.p_products.begin(), c_data.p_products.end(),
                [](ProductData a, ProductData b)
                {
                    return a.product_total > b.product_total;
                }
            );
            
        }

        seasons_data.push_back(c_data);

        std::sort(seasons_data.begin(), seasons_data.end(),
            [](CategoryData a, CategoryData b)
            {
                return a.category_total > b.category_total;
            }
        );
    }

    return seasons_data;
}


std::vector<CategoryData> get_age_groups_data(mongocxx::collection products)
{
    std::vector<CategoryData> age_groups_data;
    
    bsoncxx::stdx::optional<bsoncxx::document::value> my_components = products.find_one(document{} << "name" << "Components" << finalize);
    bsoncxx::document::view  my_components_view = my_components.value().view();

    bsoncxx::stdx::optional<bsoncxx::document::value> product;
    bsoncxx::document::view product_view;

    bsoncxx::array::view mage_groups = my_components_view["Age Groups"].get_array();
    bsoncxx::array::view mseasons = my_components_view["Seasons"].get_array();
    bsoncxx::array::view mproducts = my_components_view["Products"].get_array();

    bsoncxx::array::view mbrands;
    bsoncxx::array::view mquantities;
   
    for (bsoncxx::array::element m_age_group : mage_groups)
    {
        CategoryData c_data;
        c_data.category_name = m_age_group.get_string().value.to_string();

        for (bsoncxx::array::element m_product : mproducts)
        {
            ProductData p_data;
            p_data.product_name = m_product.get_string().value.to_string();

            product = products.find_one(document{} << "name" << m_product.get_string().value.to_string().c_str() << finalize);
            product_view = product.value().view();

            mbrands = my_components_view["Brands"][m_product.get_string().value.to_string()].get_array();

            for (bsoncxx::array::element m_brand : mbrands)
            {
                BrandData b_data;
                b_data.brand_name = m_brand.get_string().value.to_string();

                size_t name_separator_index = b_data.brand_name.find_first_of('_', 0);
                b_data.brand_name = b_data.brand_name.substr(name_separator_index + 1, b_data.brand_name.size() - name_separator_index);

                mquantities = my_components_view["Quantities"][m_brand.get_string().value.to_string()].get_array();

                for (bsoncxx::array::element m_quantity : mquantities)
                {
                    QuantityData q_data;
                    q_data.quantity_name = m_quantity.get_string().value.to_string();
                    for (bsoncxx::array::element m_season : mseasons)
                    {                        
                        q_data.quantity_total += product_view["Brands"][m_brand.get_string().value.to_string()][m_quantity.get_string().value.to_string()][m_season.get_string().value.to_string()][m_age_group.get_string().value.to_string()]["Total"].get_int32();
                        
                    }

                    b_data.brand_total += q_data.quantity_total;//update brand total
                    b_data.b_quantities.push_back(q_data);
                    std::sort(b_data.b_quantities.begin(), b_data.b_quantities.end(),
                        [](QuantityData a, QuantityData b)
                        {
                            return a.quantity_total > b.quantity_total;
                        }
                    );

                }
                p_data.product_total += b_data.brand_total; //update product total
                p_data.p_brands.push_back(b_data);

                std::sort(p_data.p_brands.begin(), p_data.p_brands.end(),
                    [](BrandData a, BrandData b)
                    {
                        return a.brand_total > b.brand_total;
                    }
                );

            }
            c_data.category_total += p_data.product_total;
            c_data.p_products.push_back(p_data);
            std::sort(c_data.p_products.begin(), c_data.p_products.end(),
                [](ProductData a, ProductData b)
                {
                    return a.product_total > b.product_total;
                }
            );           
        }

        age_groups_data.push_back(c_data);

        std::sort(age_groups_data.begin(), age_groups_data.end(),
            [](CategoryData a, CategoryData b)
            {
                return a.category_total > b.category_total;
            }
        );
    }

    return age_groups_data;
}


std::vector<CategoryData> get_genders_data(mongocxx::collection products)
{
    std::vector<CategoryData> genders_data;    

    bsoncxx::stdx::optional<bsoncxx::document::value> my_components = products.find_one(document{} << "name" << "Components" << finalize);
    bsoncxx::document::view  my_components_view = my_components.value().view();

    bsoncxx::stdx::optional<bsoncxx::document::value> product;
    bsoncxx::document::view product_view;

    bsoncxx::array::view mgenders = my_components_view["Genders"].get_array();
    bsoncxx::array::view mage_groups = my_components_view["Age Groups"].get_array();
    bsoncxx::array::view mseasons = my_components_view["Seasons"].get_array();
    bsoncxx::array::view mproducts = my_components_view["Products"].get_array();

    bsoncxx::array::view mbrands;
    bsoncxx::array::view mquantities;
    
    for (bsoncxx::array::element m_gender : mgenders)
    {
        CategoryData c_data;
        c_data.category_name = m_gender.get_string().value.to_string();

        for (bsoncxx::array::element m_product : mproducts)
        {
            ProductData p_data;
            p_data.product_name = m_product.get_string().value.to_string();

            product = products.find_one(document{} << "name" << m_product.get_string().value.to_string().c_str() << finalize);
            product_view = product.value().view();

            mbrands = my_components_view["Brands"][m_product.get_string().value.to_string()].get_array();

            for (bsoncxx::array::element m_brand : mbrands)
            {
                BrandData b_data;
                b_data.brand_name = m_brand.get_string().value.to_string();

                size_t name_separator_index = b_data.brand_name.find_first_of('_', 0);
                b_data.brand_name = b_data.brand_name.substr(name_separator_index + 1, b_data.brand_name.size() - name_separator_index);

                mquantities = my_components_view["Quantities"][m_brand.get_string().value.to_string()].get_array();

                for (bsoncxx::array::element m_quantity : mquantities)
                {
                    QuantityData q_data;
                    q_data.quantity_name = m_quantity.get_string().value.to_string();
                    for (bsoncxx::array::element m_season : mseasons)
                    {
                        for (bsoncxx::array::element m_age_group : mage_groups)
                        {
                            q_data.quantity_total += product_view["Brands"][m_brand.get_string().value.to_string()][m_quantity.get_string().value.to_string()][m_season.get_string().value.to_string()][m_age_group.get_string().value.to_string()][m_gender.get_string().value.to_string()]["Total"].get_int32();
                            
                        }                        
                    }

                    b_data.brand_total += q_data.quantity_total;//update brand total
                    b_data.b_quantities.push_back(q_data);

                    std::sort(b_data.b_quantities.begin(), b_data.b_quantities.end(),
                        [](QuantityData a, QuantityData b)
                        {
                            return a.quantity_total > b.quantity_total;
                        }
                    );

                }
                p_data.product_total += b_data.brand_total; //update product total
                p_data.p_brands.push_back(b_data);

                std::sort(p_data.p_brands.begin(), p_data.p_brands.end(),
                    [](BrandData a, BrandData b)
                    {
                        return a.brand_total > b.brand_total;
                    }
                );
            }

            c_data.category_total += p_data.product_total;
            c_data.p_products.push_back(p_data);

            std::sort(c_data.p_products.begin(), c_data.p_products.end(),
                [](ProductData a, ProductData b)
                {
                    return a.product_total > b.product_total;
                }
            );
            
        }
        
        genders_data.push_back(c_data);

        std::sort(genders_data.begin(), genders_data.end(),
            [](CategoryData a, CategoryData b)
            {
                return a.category_total > b.category_total;
            }
        );
    }

    return genders_data;
}

std::vector<CategoryData> get_locations_data(mongocxx::collection products)
{
    std::vector<CategoryData> locations_data;
    
    bsoncxx::stdx::optional<bsoncxx::document::value> my_components = products.find_one(document{} << "name" << "Components" << finalize);
    bsoncxx::document::view  my_components_view = my_components.value().view();

    bsoncxx::stdx::optional<bsoncxx::document::value> product;
    bsoncxx::document::view product_view;

    bsoncxx::array::view mgenders = my_components_view["Genders"].get_array();
    bsoncxx::array::view mlocations = my_components_view["Locations"].get_array();
    bsoncxx::array::view mage_groups = my_components_view["Age Groups"].get_array();
    bsoncxx::array::view mseasons = my_components_view["Seasons"].get_array();
    bsoncxx::array::view mproducts = my_components_view["Products"].get_array();

    bsoncxx::array::view mbrands;
    bsoncxx::array::view mquantities;

    
    for (bsoncxx::array::element m_location : mlocations)
    {
        CategoryData c_data;
        c_data.category_name = m_location.get_string().value.to_string();

        for (bsoncxx::array::element m_product : mproducts)
        {
            ProductData p_data;
            p_data.product_name = m_product.get_string().value.to_string();

            product = products.find_one(document{} << "name" << m_product.get_string().value.to_string().c_str() << finalize);
            product_view = product.value().view();

            mbrands = my_components_view["Brands"][m_product.get_string().value.to_string()].get_array();

            for (bsoncxx::array::element m_brand : mbrands)
            {
                BrandData b_data;
                b_data.brand_name = m_brand.get_string().value.to_string();

                size_t name_separator_index = b_data.brand_name.find_first_of('_', 0);
                b_data.brand_name = b_data.brand_name.substr(name_separator_index + 1, b_data.brand_name.size() - name_separator_index);

                mquantities = my_components_view["Quantities"][m_brand.get_string().value.to_string()].get_array();

                for (bsoncxx::array::element m_quantity : mquantities)
                {
                    QuantityData q_data;
                    q_data.quantity_name = m_quantity.get_string().value.to_string();
                    for (bsoncxx::array::element m_season : mseasons)
                    {
                        for (bsoncxx::array::element m_age_group : mage_groups)
                        {
                            for (bsoncxx::array::element m_gender : mgenders)
                            {
                                
                                q_data.quantity_total += product_view["Brands"][m_brand.get_string().value.to_string()][m_quantity.get_string().value.to_string()][m_season.get_string().value.to_string()][m_age_group.get_string().value.to_string()][m_gender.get_string().value.to_string()][m_location.get_string().value.to_string()].get_int32();
                                                                
                            }                            
                        }
                    }
                    b_data.brand_total += q_data.quantity_total;//update brand total
                    b_data.b_quantities.push_back(q_data);

                    std::sort(b_data.b_quantities.begin(), b_data.b_quantities.end(),
                        [](QuantityData a, QuantityData b)
                        {
                            return a.quantity_total > b.quantity_total;
                        }
                    );
                }
                p_data.product_total += b_data.brand_total; //update product total
                p_data.p_brands.push_back(b_data);

                std::sort(p_data.p_brands.begin(), p_data.p_brands.end(),
                    [](BrandData a, BrandData b)
                    {
                        return a.brand_total > b.brand_total;
                    }
                );
            }
            c_data.category_total += p_data.product_total;
            c_data.p_products.push_back(p_data);

            std::sort(c_data.p_products.begin(), c_data.p_products.end(),
                [](ProductData a, ProductData b)
                {
                    return a.product_total > b.product_total;
                }
            );           

        }

        locations_data.push_back(c_data);

        std::sort(locations_data.begin(), locations_data.end(),
            [](CategoryData a, CategoryData b)
            {
                return a.category_total > b.category_total;
            }
        );
    }

    return locations_data;
}


std::vector<ProductData> get_products_sales_data(mongocxx::collection products)
{
    std::vector<ProductData> products_data;

    bsoncxx::stdx::optional<bsoncxx::document::value> my_components = products.find_one(document{} << "name" << "Components" << finalize);
    bsoncxx::document::view  my_components_view = my_components.value().view();

    bsoncxx::stdx::optional<bsoncxx::document::value> product;
    bsoncxx::document::view product_view;

    bsoncxx::array::view mproducts = my_components_view["Products"].get_array();

    bsoncxx::array::view mbrands;
    bsoncxx::array::view mquantities;

   for (bsoncxx::array::element m_product : mproducts)
     {
         ProductData p_data;
         p_data.product_name = m_product.get_string().value.to_string();

         product = products.find_one(document{} << "name" << m_product.get_string().value.to_string().c_str() << finalize);
         product_view = product.value().view();

         p_data.product_total = product_view["Brands"]["Total"].get_int32();

         mbrands = my_components_view["Brands"][m_product.get_string().value.to_string()].get_array();

         for (bsoncxx::array::element m_brand : mbrands)
         {
             BrandData b_data;
             b_data.brand_name = m_brand.get_string().value.to_string();
             size_t name_separator_index = b_data.brand_name.find_first_of('_', 0);
             b_data.brand_name = b_data.brand_name.substr(name_separator_index + 1, b_data.brand_name.size() - name_separator_index);

             b_data.brand_total = product_view["Brands"][m_brand.get_string().value.to_string()]["Total"].get_int32();

             mquantities = my_components_view["Quantities"][m_brand.get_string().value.to_string()].get_array();

             for (bsoncxx::array::element m_quantity : mquantities)
             {
                 QuantityData q_data;
                 q_data.quantity_name = m_quantity.get_string().value.to_string();
                 q_data.quantity_total = product_view["Brands"][m_brand.get_string().value.to_string()][m_quantity.get_string().value.to_string()]["Total"].get_int32();

                 b_data.b_quantities.push_back(q_data);

                 std::sort(b_data.b_quantities.begin(), b_data.b_quantities.end(),
                     [](QuantityData a, QuantityData b)
                     {
                         return a.quantity_total > b.quantity_total;
                     }
                 );

             }             
             p_data.p_brands.push_back(b_data);

             std::sort(p_data.p_brands.begin(), p_data.p_brands.end(),
                 [](BrandData a, BrandData b)
                 {
                     return a.brand_total > b.brand_total;
                 }
             );
         }          
        

         products_data.push_back(p_data);

         std::sort(products_data.begin(), products_data.end(),
             [](ProductData a, ProductData b)
             {
                 return a.product_total > b.product_total;
             }
         );
     }
    

    return products_data;
}


std::vector<std::vector<std::string>> get_sales_data(mongocxx::collection products)
{
    std::vector<std::vector<std::string>> details;
    std::vector<std::string> data = {};
    for (int count = 0; count < 10; count++)
        details.push_back(data);

    bsoncxx::stdx::optional<bsoncxx::document::value> sales_data = products.find_one(document{} <<"name" << "SalesData" << finalize);
    bsoncxx::document::view sales_data_view = sales_data.value().view();

    bsoncxx::array::view indices = sales_data_view["indices"].get_array();
    int item_count = 0;
    for (bsoncxx::array::element index : indices)
    {
        std::string product_name = sales_data_view[index.get_string().value.to_string()]["Product"].get_string().value.to_string();
        std::string date_time = product_name;//product name contains both the name and the date separated by an underscore

        size_t underscore_pos = product_name.find_first_of('_', 0);
        product_name = product_name.substr(0, underscore_pos);
        date_time = date_time.substr(underscore_pos + 1, date_time.size() - underscore_pos);

        details[0].push_back(product_name);

        std::string brand_name = sales_data_view[index.get_string().value.to_string()]["Brand"].get_string().value.to_string();
        size_t product_and_brand_separator_index = brand_name.find('_', 0); //the brand name is a combination of the product name and the brand name separated by an underscore
        brand_name = brand_name.substr(product_and_brand_separator_index + 1, brand_name.size() - product_and_brand_separator_index);
        details[1].push_back(brand_name);

        details[2].push_back(sales_data_view[index.get_string().value.to_string()]["Quantity"].get_string().value.to_string());
        details[3].push_back(sales_data_view[index.get_string().value.to_string()]["Season"].get_string().value.to_string());
        details[4].push_back(sales_data_view[index.get_string().value.to_string()]["Age Group"].get_string().value.to_string());
        details[5].push_back(sales_data_view[index.get_string().value.to_string()]["Gender"].get_string().value.to_string());
        details[6].push_back(sales_data_view[index.get_string().value.to_string()]["Location"].get_string().value.to_string());

        item_count = sales_data_view[index.get_string().value.to_string()]["Item Count"].get_int32();
        details[7].push_back(std::to_string(item_count));

        details[8].push_back(index.get_string().value.to_string());

        details[9].push_back(date_time);

    }

    return details;
}


std::vector<UserData> get_users_data(mongocxx::collection login)
{
    std::vector<UserData> details;

    bsoncxx::stdx::optional<bsoncxx::document::value> my_login_data = login.find_one(document{} << "name" << "LoginData" << finalize);
    bsoncxx::document::view  my_login_data_view = my_login_data.value().view();

    UserData udata;

    udata.level = "Admin";
    udata.password = my_login_data_view["Admin"].get_string().value.to_string();

    details.push_back(udata);

    udata.level = "Finance";
    udata.password = my_login_data_view["Finance"].get_string().value.to_string();

    details.push_back(udata);

    udata.level = "Marketer";
    udata.password = my_login_data_view["Marketer"].get_string().value.to_string();

    details.push_back(udata);

    udata.level = "Regular";
    udata.password = my_login_data_view["Regular"].get_string().value.to_string();

    details.push_back(udata);

    return details;
}


int set_password(mongocxx::collection login, std::string user, std::string new_password)
{
    login.update_one(make_document(kvp("name", "LoginData")), make_document(kvp("$set", make_document(kvp(user, new_password)))));

    return 1;
}